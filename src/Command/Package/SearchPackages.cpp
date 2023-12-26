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
    json raw_index = fetchIndex();
    Utils::toLower(query);

    const float star_weight = 0.01;
    const float name_weight = 10;

    for(Package package: raw_index){

      int score = 0;

      score += (int)((float)Utils::getStringScore(package.name, query) * name_weight);
      score += (int)((float)package.stars * star_weight);
      score += Utils::getStringScore(package.description, query);
      score += Utils::getStringScore(package.target_link, query);

      results.emplace_back(package, score);
    }

    std::sort(results.begin(), results.end(), [](
          std::pair<Package,int> package_a,
          std::pair<Package,int> package_b){
        return package_a.second > package_b.second;
        });

    return results;
  }

  std::vector<Package> filterOutBest(std::vector<std::pair<Package,int>> &packages){

    std::vector<Package> filter_results_vec;
    int total_score = 0;

    for(auto &package: packages){
      total_score += package.second;
    }

    float average_score = (static_cast<float>(total_score) / static_cast<float>(packages.size()));
    const float threshold_factor = 1.3;
    const int threshold = static_cast<int>(average_score * threshold_factor);
    for(auto &package: packages){
      if(package.second > (threshold)){
        filter_results_vec.emplace_back(package.first);
      }
    }


    return filter_results_vec;
  }

  std::pair<bool, Package> getExactMatch(std::string &query){
    json raw_index = fetchIndex();
    Utils::toLower(query);
    for(Package package: raw_index){
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
    }      
    query = inter->args->operator[]("query").as<std::string>();
    auto results = calculatePackageScores(query);
    List package_list{};
    package_list.ReverseIndexed();
    for(auto result: results){
      package_list.pushBack(
          ListItem(
            result.first.name + " (" + result.first.git + ")",
            result.first.description));
    }

    std::cout << package_list.Build() << std::endl;

    return true;
   
  }

  std::vector<std::pair<Package,int>> _search(std::string &query){
    auto results = calculatePackageScores(query);
    return results;
  }

  std::pair<bool, Package> searchGetFirst(std::string& query){
    auto results = calculatePackageScores(query);
    if(results.empty()){
      return {false, Package()};
    }
    return {true, results[0].first};
  }

  std::pair<bool, Dependency> searchWithPrompt(std::string& query, bool latest){
    auto results = calculatePackageScores(query);
    if(results.empty()){
      return {false, Dependency()};
    }
    List package_list{}; 
    package_list.Numbered().
      ReverseIndexed();
    for(auto &result: results){
      package_list.pushBack(
          ListItem(
            result.first.name + " (" + result.first.git + ")",
            result.first.description));
    }


    std::cout << package_list.Build() << std::endl;

    std::vector<Package> filter_results_vec = filterOutBest(results);

    Prompt prompt("Select a package to install: ");
    for(size_t i = 0; i < filter_results_vec.size(); i++){
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
