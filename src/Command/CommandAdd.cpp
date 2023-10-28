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
  bool add(std::shared_ptr<Context> &ctx, cxxopts::ParseResult &args) {
    args.count("subcommand");
    if (args.count("subcommand") != 0) {
      std::string subcommand = args["subcommand"].as<std::string>();
      if (subcommand == "dep") {
        addDependency(ctx, args);
      }
      if (subcommand == "flag") {
        addFlag(ctx, args);
      }
    }
    return true;
  }
  bool addFlag(std::shared_ptr<Context> ctx, cxxopts::ParseResult &args) {
    if (args.count("help")) {
      std::cout << "Usage: addFlag [options] flags" << std::endl;
    }
    if (args.count("subcommand") == 0) {
      for (auto flag : args["subcommand"].as<std::vector<std::string>>()) {
        ctx->flags.push_back(flag);
      }
    }
    return true;
  }

  nlohmann::json fetch(cpr::Url url) {
    cpr::Response r = cpr::Get(cpr::Url{url});
    // add error handling
    r.status_code == 200
      ? [&url](cpr::Response r) -> void{
        std::cout << r.error.message << std::endl;
        std::cout << "couldn't connect to " << url  << std::endl;
        exit(1);
      }(r)
    : [&url](cpr::Response r) -> void{
      std::cout << r.status_code << std::endl;
      std::cout << "fetching data from:"  << url << std::endl;
    }(r);
    try {
      nlohmann::json json = nlohmann::json::parse(r.text);
      return json;
    } catch (const std::exception &e) {
      std::cout << "Error: " << e.what() << std::endl;
      exit(1);
    }
  }

  std::string downloadIndex() {
    std::cout << "Downloading index.json" << std::endl;
    cpr::Response r = cpr::Get(cpr::Url{"https://github.com/cmaker-dev/index/releases/latest/download/index.json"});
    std::cout << r.status_code << std::endl;
    return r.text;
  }
  typedef struct packageResult {
    std::string name;
    std::string url;
    std::string version;
    int score;
  } packageResult;
  std::vector<packageResult> calculatePackageScores(std::vector<std::string> queryTokens){
    std::vector<packageResult> results;
    std::string index = downloadIndex();
    json rawIndex = json::parse(index);
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
  }

  bool addDependency(std::shared_ptr<Context> ctx, cxxopts::ParseResult &args) {
    for(auto arg: args.arguments()){
      std::cout << arg.key() << std::endl;
    }
    if (args.count("subcommand") > 2) {
      std::cout << "Usage: add dep [options] name url branch" << std::endl;
      return false;
    }else{

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

      if(checkForOverlappingDependencies(ctx, searchResults[index].name)){
        std::cout << "Package already installed" << std::endl;
        return false;
      }

      std::cout << "Adding dependency to config.toml" << std::endl;
      ctx->dependencies.push_back({

        .name = searchResults[index].name,
        .url = searchResults[index].url,
        .version = "origin/main",
        //TODO: Change target link to be the actual link
        .target_link = searchResults[index].name

      });
      std::cout << "Writing config.toml" << std::endl;
      Generators::ConfigToml::writeConfig(ctx);

      return true;
    }



    // nlohmann::json json = fetch(
    //     "https://github.com/cmaker-dev/index/releases/latest/download/index.json");


//     for (nlohmann::json package : json) {
//       if (package["Name"] == args["args"].as<std::vector<std::string>>()[0]) {
//         dependency dependency_new = {
//           .name = static_cast<std::string>(package_data["Name"]),
//           .url = static_cast<std::string>(package_data["Homepage"]),
//           .version = static_cast<std::string>(package_data["Tag"]),
//           .target_link = static_cast<std::string>(package_data["Link"])};
// 
//         if (std::any_of(ctx->dependencies.begin(), ctx->dependencies.end(),
//               [&dependency_new](dependency &dep) {
//               return dep.name == dependency_new.name;
//               })) {
//           std::cout << "Dependency already exists not adding" << std::endl;
//         } else {
//           ctx->dependencies.push_back(dependency_new);
//         }
//       }
    // }
    std::cout << ctx->dependencies.size() << std::endl;
    Generators::ConfigToml::writeConfig(ctx);
    Generators::CMakeList::create(ctx);
    return true;
  }
} // namespace Command
