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


  bool searchVersions(){
    std::cout << "Getting versions" << std::endl;
    return true;
  }


  bool Interface::addDependency() {
    if (args->count("args") == 0) {
      std::cout << 
        "Usage add dep:" << ENDL
        "\t[args]: the dependencies to project" << ENDL
        "\tcmake add dep [args] " << std::endl;
      return false;
    }

    std::string query = args->operator[]("args").as<std::vector<std::string>>()[0];
    std::vector<Package> searchResults = searchPackage(query);
    if(searchResults.size() == 0){
      std::cout << "No results found" << std::endl;
      return false;
    }

    std::cout << "Select a package to install: ";
    std::string input;
    std::cin >> input;
    int index;
    try{
      index = std::stoi(input);
    }
catch(...){
      std::cout << "Invalid input" << std::endl;
      return false;
    }


    std::cout << "Installing " << searchResults[index].name << std::endl;

    //json versionJson = Utils::fetchJson("https://raw.githubusercontent.com/cmaker-dev/index/main/index/" + searchResults[index].name + "/info.json"); 
    json versionJson = fetchIndex(); 
  
    std::vector<std::string> versions = searchResults[index].versions;
    std::string version = ""; 
    for(size_t i = searchResults[index].versions.size(); i > -1; i--){
      std::cout << "[" << termcolor::green << i << termcolor::white << "]" << searchResults[index].versions[i] << std::endl;
      if (searchResults[index].versions[i] == "master" || searchResults[index].versions[i] == "main"  ||  searchResults[index].versions[i] == "stable"){
        version = searchResults[index].versions[i];
      }

    }


    json packageInfo = json{{"name", searchResults[index].name}
, {"url", searchResults[index].url}
, {"versions", searchResults[index].versions}
, {"target_link", searchResults[index].target_link}
}
;
    List* list = (new List())->Numbered()->ReverseIndexed();
    for(size_t i = 0; i < searchResults[index].versions.size(); i++){
      list->pushBack(ListItem(searchResults[index].versions[i]));
    }

    std::cout << list->Build() << std::endl;
    std::cout << "Select a version to install [" << termcolor::green <<  version << termcolor::white << "] : ";
    std::string versionInput;
    std::cin >> versionInput;

    int versionIndex;
    try{
      versionIndex = std::stoi(versionInput);
    }
catch(...){
      std::cout << "Invalid input" << std::endl;
      return false;
    }

    version = searchResults[index].versions[versionIndex];
  
    if(checkForOverlappingDependencies(pro, searchResults[index].name)){
      std::cout << "Package already installed" << std::endl;
      return false;
    }


    std::cout << "Adding dependency to config.json" << std::endl;
    pro->dependencies.push_back({

      .name = searchResults[index].name,
      .url = searchResults[index].url,
      .version = searchResults[index].versions[versionIndex],
      //TODO: Change target link to be the actual link
      .target_link = searchResults[index].target_link == "" 
        ? searchResults[index].name
        : searchResults[index].target_link

    }
);
    std::cout << "Writing config.json" << std::endl;
    if(!Generators::ConfigJson::writeConfig(pro)){
      std::cout << "Failed to write config.json" << std::endl;
    }

    if(!Generators::CMakeList::create(pro)){
      std::cout << "Failed to write CMakeLists.txt" << std::endl;
    }


    return true;
  }

}
 // namespace Command
