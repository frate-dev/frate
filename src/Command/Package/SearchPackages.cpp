#include <Frate/Command/Package.hpp>
#include <Frate/Utils/CLI.hpp>
#include <Frate/Utils/General.hpp> 
#include <Frate/Command/CommandMode.hpp>
#include <Frate/Command.hpp>
#include "Frate/Dependency.hpp"

namespace Frate::Command::Packages {
  using namespace Utils::CLI;

  std::vector<std::pair<Package,int>> calculatePackageScores(std::string &query){
    std::vector<std::pair<Package,int>> results;
    json rawIndex = fetchIndex();
    Utils::toLower(query);


    for(Package package: rawIndex){

      int score = 0;
      score += Utils::getStringScore(package.name, query) * 10;
      score += package.stars / 100;
      score += Utils::getStringScore(package.description, query);
      score += Utils::getStringScore(package.target_link, query);

      results.push_back({package, score});
    }

    std::sort(results.begin(), results.end(), [](
          std::pair<Package,int> a,
          std::pair<Package,int> b){
        return a.second > b.second;
        });

    return results;
  }

  std::vector<Package> filterOutBest(std::vector<std::pair<Package,int>> &packages){

    std::vector<Package> filterResultsVec;
    int totalScore = 0;

    for(auto &package: packages){
      totalScore += package.second;
    }

    int averageScore = totalScore / packages.size();

    for(auto &package: packages){
      if(package.second > ( averageScore * 1.3 )){
        filterResultsVec.emplace_back(package.first);
      }
    }


    return filterResultsVec;
  }

  std::pair<bool, Package> getExactMatch(std::string &query){
    json rawIndex = fetchIndex();
    Utils::toLower(query);
    for(Package package: rawIndex){
      if(package.name == query){
        return {true, package};
      }
    }
    return {false, Package()};
  }

  bool search(std::shared_ptr<Interface> inter){
    std::string query;
    if(inter->args->operator[]("query").count() == 0){
      std::cout << "No query provided" << std::endl;
      std::cout << "Usage: frate search p <query>" << std::endl;
      return false;
    }else{
      query = inter->args->operator[]("query").as<std::string>();
      auto results = calculatePackageScores(query);
      List packageList{};
        packageList.ReverseIndexed();
      for(auto result: results){
        packageList.pushBack(
            ListItem(
              result.first.name + " (" + result.first.git + ")",
              result.first.description));
      }

      std::cout << packageList.Build() << std::endl;

      return true;
    }
  }

  std::vector<std::pair<Package,int>> _search(std::string &query){
    auto results = calculatePackageScores(query);
    return results;
  }

  std::pair<bool, Package> searchGetFirst(std::string& query){
    auto results = calculatePackageScores(query);
    if(results.size() == 0){
      return {false, Package()};
    }
    return {true, results[0].first};
  }

  std::pair<bool, Dependency> searchWithPrompt(std::string& query, bool latest){
    auto results = calculatePackageScores(query);
    if(results.size() == 0){
      return {false, Dependency()};
    }
    List packageList{}; 
    packageList.Numbered().
      ReverseIndexed();
    for(auto &result: results){
      packageList.pushBack(
          ListItem(
            result.first.name + " (" + result.first.git + ")",
            result.first.description));
    }


    std::cout << packageList.Build() << std::endl;

    std::vector<Package> filterResultsVec = filterOutBest(results);

    Prompt prompt("Select a package to install: ");
    for(size_t i = 0; i < filterResultsVec.size(); i++){
      prompt.addOption(i);
    }
    prompt.run();
    auto[valid, index] = prompt.get<int>();
    if(!valid){
      return {false, Dependency()};
    }
    Package chosen_package = results[index].first;
    Dependency dep;
    
    if(latest){
      return {true, Dependency(chosen_package, chosen_package.versions[0])};
    }

    std::string version = promptForVersion(chosen_package);

    return {true, Dependency(chosen_package, version)};
  }
}
