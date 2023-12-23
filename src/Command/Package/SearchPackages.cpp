#include <Frate/Command/Package.hpp>
#include <Frate/Utils/CLI.hpp>
#include <Frate/Utils/General.hpp> 
#include <Frate/Command/CommandMode.hpp>
#include <Frate/Command.hpp>

namespace Frate::Command::Packages {
  using namespace Utils::CLI;

  std::vector<Package> calculatePackageScores(std::string &query){
    std::vector<Package> results;
    json rawIndex = fetchIndex();
    Utils::toLower(query);


    for(Package package: rawIndex){

      int score = 0;
      score += Utils::getStringScore(package.name, query) * 10;
      score += package.stars / 100;
      score += Utils::getStringScore(package.description, query);
      score += Utils::getStringScore(package.target_link, query);
      package.score = score;

      results.push_back(package);
    }

    std::sort(results.begin(), results.end(), [](Package a, Package b){
        return a.score > b.score;
        });

    return results;
  }

  std::vector<Package> filterOutBest(std::vector<Package> &packages){

    std::vector<Package> filterResultsVec;

    int totalScore = std::accumulate(
        packages.begin(), packages.end(), 0, [](int a, Package b){
        return a + b.score;
        });

    int averageScore = totalScore / packages.size();

    for(Package package: packages){
      if(package.score > averageScore){
        filterResultsVec.push_back(package);
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
      std::vector<Package> results = calculatePackageScores(query);
      List packageList{};
        packageList.ReverseIndexed();
      for(Package result: results){
        packageList.pushBack(ListItem(result.name + " (" + result.git + ")", result.description));
      }

      std::cout << packageList.Build() << std::endl;

      return true;
    }
  }

  std::vector<Package> _search(std::string &query){
    std::vector<Package> results = calculatePackageScores(query);
    return results;
  }

  std::pair<bool, Package> searchGetFirst(std::string& query){
    std::vector<Package> results = calculatePackageScores(query);
    if(results.size() == 0){
      return {false, Package()};
    }
    return {true, results[0]};
  }

  std::pair<bool, Package> searchWithPrompt(std::string& query, bool latest){
    std::vector<Package> results = calculatePackageScores(query);
    if(results.size() == 0){
      return {false, Package()};
    }
    List packageList{}; 
    packageList.Numbered().
      ReverseIndexed();
    for(Package result: results){
      packageList.pushBack(ListItem(result.name + " (" + result.git + ")", result.description));
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
      return {false, Package()};
    }
    Package chosen_package = results[index];
    
    if(latest){
      chosen_package.selected_version = chosen_package.versions[0];
      return {true, chosen_package};
    }

    std::string version = promptForVersion(chosen_package);

    chosen_package.selected_version = version;

    return {true, chosen_package};
  }
}
