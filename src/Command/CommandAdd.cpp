#include "../Generators/Generators.hpp"
#include "Command.hpp"
#include <algorithm>
#include <cxxopts.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include "../Utils/General.hpp"
#include "../Utils/CLI.hpp"
#include <cmath>
#include <termcolor/termcolor.hpp>

namespace Command {
  using nlohmann::json;
  using Utils::CLI::List;
  using Utils::CLI::ListItem;
  bool Interface::add() {
    if (!(args->count("subcommand") > 0)) {
      std::cout << "Usage add:" << ENDL
        "\tdep: adds a dependency" << ENDL
        "\tflag: adds a flag" << ENDL
        "\tlib:  adds a library" << std::endl;
      return false;
    }

      std::string subcommand = args->operator[]("subcommand").as<std::string>();
      if (subcommand == "dep") {
        this->addDependency();
      }

      if (subcommand == "flag") {
        this->addFlag();
      }


    return true;
  }


  bool Interface::addFlag() {
    if (args->count("subcommand") == 0) {
      for (auto flag : args->operator[]("subcommand").as<std::vector<std::string>>()) {
        pro->flags.push_back(flag);
      }

    }

    return true;
  }


  bool Interface::addAuthors(){
    if (pro->args->count("args") == 0) {
      for (auto author : pro->args->operator[]("args").as<std::vector<std::string>>()) {
        pro->authors.push_back(author);
      }

    }

    return true;
  }


  bool checkForOverlappingDependencies(std::shared_ptr<Project> pro, std::string name){
    if(pro->dependencies.size() == 0){
      return false;
    }
    for(Dependency dep: pro->dependencies){
      if(dep.name == name){
        return true;
      }

    }

    return false;
  }
  Package promptPackageSearchResults(std::string query){

    std::vector<Package> searchResults = searchPackage(query);
    if(searchResults.size() == 0){
      std::cout << "No results found" << std::endl;
      exit(0);
    }

    List *packageList = (new Utils::CLI::List())->
      Numbered()->
      ReverseIndexed();
    for(Package result: searchResults){
      packageList->pushBack(ListItem(result.name + " (" + result.git + ")", result.description));
    }
    std::cout << packageList->Build() << std::endl;
    Prompt<int> *prompt = new Prompt<int>("Select a package to install: ");
    for(size_t i = 0; i < searchResults.size(); i++){
      prompt->AddOption(i);
    }
    prompt->Run();
    int index = prompt->Get();
    
    return searchResults[index];
  }



  bool Interface::addDependency() {
    bool exact = false;
    bool latest = false;
    if (args->count("args") == 0) {
      std::cout << 
        "Usage add dep:" << ENDL
        "\t[args]: the dependencies to project" << ENDL
        "\tcmake add dep [args] " << std::endl;
      return false;
    }
    if(args->operator[]("exact").as<bool>()){
      exact = true;
    }
    if(args->operator[]("latest").as<bool>()){
      latest = true;
    }

    std::string query = args->operator[]("args").as<std::vector<std::string>>()[0];
    
    
    Package chosen_package;
    if(!exact){
      chosen_package = promptPackageSearchResults(query);
    }else{
      chosen_package = getExactPackage(query);
    }


    std::cout << "Installing " << chosen_package.name << std::endl;

    //json versionJson = Utils::fetchJson("https://raw.githubusercontent.com/cmaker-dev/index/main/index/" + searchResults[index].name + "/info.json"); 
  
    std::string version = ""; 
    std::reverse(chosen_package.versions.begin(), chosen_package.versions.end());
    std::vector<std::string> versions = chosen_package.versions;

// WHY IS THIS STILL HERE?
//     json packageInfo = json{{"name", searchResults[index].name}
// , {"url", searchResults[index].git}
// , {"versions", searchResults[index].versions}
// , {"target_link", searchResults[index].target_link}
// }
// ;
    List* list = (new List())->Numbered()->ReverseIndexed();
    for(size_t i = 0; i < chosen_package.versions.size(); i++){
      list->pushBack(ListItem(chosen_package.versions[i]));
    }

    std::cout << list->Build() << std::endl;

    Prompt<int> *prompt = new Prompt<int>("Select a version to install: ");

    for(size_t i = 0; i < chosen_package.versions.size(); i++){
      prompt->AddOption(i);
    }

    prompt->ExitOnFailure()->Run();


    version = chosen_package.versions[prompt->Get()];
  
    if(checkForOverlappingDependencies(pro, chosen_package.name)){
      std::cout << "Package already installed" << std::endl;
      return false;
    }


    std::cout << "Adding dependency to config.json" << std::endl;
    //Reflecing the package to dependency
    //TODO: Stop this shit
    pro->dependencies.push_back({
      .name = chosen_package.name,
      .git = chosen_package.git,
      .version = version,
      .target_link = chosen_package.target_link
    });


    std::cout << "Writing config.json" << std::endl;
    if(!Generators::ConfigJson::writeConfig(pro)){
      std::cout << "Failed to write config.json" << std::endl;
    }

    if(!Generators::CMakeList::createCMakeListsExecutable(pro)){
      std::cout << "Failed to write CMakeLists.txt" << std::endl;
    }


    return true;
  }

}
 // namespace Command
