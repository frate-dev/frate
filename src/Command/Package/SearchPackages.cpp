#include <Frate/Command/Package.hpp>
#include <Frate/Utils/CLI.hpp>
#include <Frate/Utils/General.hpp> 
#include <Frate/Command/CommandMode.hpp>

namespace Frate::Command::Packages {
  using namespace Utils::CLI;

  std::vector<Package> calculatePackageScores(std::string &query){
    std::vector<Package> results;
    json rawIndex = fetchIndex();
    Utils::toLower(query);


    for(json json: rawIndex){
      Package package;
      package.fromJson(json);

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

    int totalScore = std::accumulate(
        packages.begin(), packages.end(), 0, [](int a, Package b){
        return a + b.score;
        });

    int averageScore = totalScore / packages.size();

    auto filterResults = packages | std::views::filter(
        [&averageScore](Package package){
        return package.score > 2 * averageScore;
        });

    std::vector<Package> filterResultsVec(filterResults.begin(), filterResults.end());

    return filterResultsVec;
  }

  std::pair<bool, Package> getExactMatch(std::string &query){
    json rawIndex = fetchIndex();
    Utils::toLower(query);
    for(json json: rawIndex){
      Package package;
      package.fromJson(json);
      if(package.name == query){
        return {true, package};
      }
    }
    return {false, Package()};
  }

  bool search(Interface* inter){
    std::string query;
    if(inter->args->operator[]("query").count() == 0){
      std::cout << "No query provided" << ENDL;
      std::cout << "Usage: frate search p <query>" << ENDL;
      return false;
    }else{
      query = inter->args->operator[]("query").as<std::string>();
      std::vector<Package> results = calculatePackageScores(query);
      List *packageList = (new Utils::CLI::List())->
        ReverseIndexed();
      for(Package result: results){
        packageList->pushBack(ListItem(result.name + " (" + result.git + ")", result.description));
      }

      std::cout << packageList->Build() << ENDL;

      return true;
    }
  }

  std::vector<Package> search(std::string &query){
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
    List *packageList = (new Utils::CLI::List())->
      Numbered()->
      ReverseIndexed();
    for(Package result: results){
      packageList->pushBack(ListItem(result.name + " (" + result.git + ")", result.description));
    }


    std::cout << packageList->Build() << std::endl;

    std::vector<Package> filterResultsVec = filterOutBest(results);

    Prompt *prompt = new Prompt("Select a package to install: ");
    for(size_t i = 0; i < filterResultsVec.size(); i++){
      prompt->AddOption(i);
    }
    prompt->Run();
    auto[valid, index] = prompt->Get<int>();
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
