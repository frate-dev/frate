#include "../Generators/Generators.hpp"
#include "Command.hpp"
#include <algorithm>
#include <cpr/cpr.h>
#include <cxxopts.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include "../Utils/General.hpp"
#include <cmath>

namespace Command {
  using nlohmann::json;
  bool add(std::shared_ptr<Context> ctx, cxxopts::ParseResult &args) {
    if (!(args.count("subcommand") > 0)) {
      std::cout << "Usage add:" << ENDL
        "\tdep: adds a dependency" << ENDL
        "\tflag: adds a flag" << ENDL
        "\tlib:  adds a library" << std::endl;
      return false;
    }
      std::string subcommand = args["subcommand"].as<std::string>();
      if (subcommand == "dep") {
        addDependency(ctx, args);
      }
      if (subcommand == "flag") {
        addFlag(ctx, args);
      }

    return true;
  }

  bool addFlag(std::shared_ptr<Context> ctx, cxxopts::ParseResult &args) {
    if (args.count("subcommand") == 0) {
      for (auto flag : args["subcommand"].as<std::vector<std::string>>()) {
        ctx->flags.push_back(flag);
      }
    }
    return true;
  }

  typedef struct packageResult {
    std::string name;
    std::string url;
    std::string version;
    int score;
  } packageResult;

  std::vector<packageResult> calculatePackageScores(std::vector<std::string> queryTokens){
    std::vector<packageResult> results;
    json rawIndex = Utils::fetchJson("https://github.com/cmaker-dev/index/releases/latest/download/index.json"); 
    int lengthDiffAbs = 0;
    for(std::string query: queryTokens){
      for(json package: rawIndex){
        //TODO: add description that can be searched
        results.push_back({
            .name = package["name"],
            .url = package["git"],
            .score = 0
            });
        //if it is an exact match
        if(results.back().name == query){
          results.back().score += 100;
          break;
        }
        //Check if the query is in the package name
        if(results.back().name.find(query) != std::string::npos){
          results.back().score += 50;
        }

        lengthDiffAbs = std::abs((int)results.back().name.length() - (int)query.length());
        if(results.back().score > 10){
          results.back().score -= lengthDiffAbs;
        }
      }
    }
    std::sort(results.begin(), results.end(), [](packageResult a, packageResult b){
        return a.score > b.score;
        });
    std::vector<packageResult> resultsUnique;

    for(packageResult result: results){
      if(std::any_of(resultsUnique.begin(), resultsUnique.end(), [&result](packageResult &r){
            r.score += result.score;
            return r.name == result.name;
            })){
        continue;
      }
      resultsUnique.push_back(result);
    }
    return resultsUnique;
  }

  std::vector<packageResult> searchPackage(std::string query){
    std::cout << "Searching for package" << std::endl;

    std::vector<std::string> queryTokens = Utils::split(query, ' ');

    std::vector<packageResult> results = calculatePackageScores(queryTokens);


    std::cout << "Results:" << std::endl;
    for(int i = results.size();i > -1; i--){
      if(results[i].score > 10){
        //Adjusting the indexes for the normies
        std::cout << "[" << i << "]" << results[i].name << " - (" << results[i].url << ")" << std::endl;
      }
    }

    return results;
  }


  bool checkForOverlappingDependencies(std::shared_ptr<Context> ctx, std::string name){
    if(ctx->dependencies.size() == 0){
      return false;
    }
    for(dependency dep: ctx->dependencies){
      if(dep.name == name){
        return true;
      }
    }
    return false;
  }

  bool searchVersions(){
    std::cout << "Getting versions" << std::endl;
    return true;
  }

  bool addDependency(std::shared_ptr<Context> ctx, cxxopts::ParseResult &args) {
    if (args.count("args") == 0) {
      std::cout << 
        "Usage add dep:" << ENDL
        "\t[args]: the dependencies to project" << ENDL
        "\tcmake add dep [args] " << std::endl;
      return false;
    }

    std::vector<packageResult> searchResults = searchPackage(args["args"].as<std::vector<std::string>>()[0]);
    if(searchResults.size() == 0){
      std::cout << "No results found" << std::endl;
      return false;
    }
    std::cout << "Select a package to install: ";
    std::string input;
    std::cin >> input;
    int index = std::stoi(input);

    std::cout << "Installing " << searchResults[index].name << std::endl;

    json versionJson = Utils::fetchJson("https://raw.githubusercontent.com/cmaker-dev/index/main/index/" + searchResults[index].name + "/info.json"); 
  
    std::vector<std::string> versionTokens = versionJson["versions"];
    std::string version = ""; 

    for(size_t i = 0; i < versionTokens.size(); i++){
      std::cout << "[" << i << "]" << versionTokens[i] << std::endl;
      if (versionTokens[i] ==  "master" || versionTokens[i] == "main"  ||  versionTokens[i] == "stable"){
        version = versionTokens[i];
      }
    }

    
    std::cout << "Select a version to install [" + version + "] : ";
    std::string versionInput;
    std::cin >> versionInput;

    int versionIndex = std::stoi(input);
    if (!(versionInput == "")){
      version = versionTokens[versionIndex];
    }
  
    if(checkForOverlappingDependencies(ctx, searchResults[index].name)){
      std::cout << "Package already installed" << std::endl;
      return false;
    }

    std::cout << "Adding dependency to config.toml" << std::endl;
    ctx->dependencies.push_back({

      .name = searchResults[index].name,
      .url = searchResults[index].url,
      .version = version,
      //TODO: Change target link to be the actual link
      .target_link = searchResults[index].name

    });
    std::cout << "Writing config.toml" << std::endl;
    Generators::ConfigToml::writeConfig(ctx);
    Generators::CMakeList::create(ctx);

    return true;
  }
} // namespace Command
