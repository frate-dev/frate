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



  bool addFlag(Interface *inter) {
    if (inter->args->count("subcommand") == 0) {
      for (auto flag : inter->args->operator[]("subcommand").as<std::vector<std::string>>()) {
        inter->pro->flags.push_back(flag);
      }

    }

    return true;
  }

  bool addAuthors(Interface *inter){
    if (inter->pro->args->count("args") == 0) {
      for (auto author : inter->pro->args->operator[]("args").as<std::vector<std::string>>()) {
        inter->pro->authors.push_back(author);
      }
    }
    return true;
  }


  bool checkForOverlappingDependencies(std::vector<Dependency> deps, std::string &name){
    if(deps.size() == 0){
      return false;
    }
    for(Dependency dep: deps){
      if(dep.name == name){
        return true;
      }

    }

    return false;
  }
  Package promptPackageSearchResults(std::string &query){

    std::vector<Package> searchResults = searchPackage(query);

    if(searchResults.size() == 1){
      std::cout << "Installing " << searchResults[0].name << std::endl;
      return searchResults[0];
    }

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
  
  std::string promptForVersion(Package &chosen_package){

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


    return chosen_package.versions[prompt->Get()];

  }

  Package getDependency(std::string query, std::vector<Dependency> deps){
    Package chosen_package;
    chosen_package = promptPackageSearchResults(query);
    std::string version = "";
    std::reverse(chosen_package.versions.begin(), chosen_package.versions.end());
    version = promptForVersion(chosen_package);
    chosen_package.selected_version = version;
    if(checkForOverlappingDependencies(deps, chosen_package.name)){
      std::cout << "Package already installed" << std::endl;
      exit(0);
    }
    return chosen_package;
  }


  bool addDependency(Interface* inter) {
    bool latest = false;
    //TODO: Add support for multiple dependencies
    if (inter->args->count("inter->args") == 0) {
      std::cout << 
        "Usage add dep:" << ENDL
        "\t[inter->args]: the dependencies to inter->project" << ENDL
        "\tcmake add dep [inter->args] " << std::endl;
      return false;
    }

    if(inter->args->operator[]("latest").as<bool>()){
      latest = true;
    }

    std::vector<std::string> package_names = inter->args->operator[]("inter->args").as<std::vector<std::string>>();
    for (std::string package_name : package_names) { 
      
      Package chosen_package = promptPackageSearchResults(package_name);

      std::cout << "Installing " << chosen_package.name << std::endl;

    
      std::string version = ""; 
      std::reverse(chosen_package.versions.begin(), chosen_package.versions.end());
      std::vector<std::string> versions = chosen_package.versions;

      if(!latest){
        version = promptForVersion(chosen_package);
      }else{
        if(chosen_package.versions.size() == 0){
          std::cout << "No versions found" << std::endl;
          return false;
        }
        version = chosen_package.versions[0];
      }
      if(checkForOverlappingDependencies(inter->pro->dependencies, chosen_package.name)){
        std::cout << "Package already installed" << std::endl;
        return false;
      }


      std::cout << "Adding dependency to config.json" << std::endl;
      //Reflecing the package to dependency
      //TODO: Stop this shit
      inter->pro->dependencies.push_back({
        .name = chosen_package.name,
        .git = chosen_package.git,
        .version = version,
        .target_link = chosen_package.target_link
      });


      std::cout << "Writing config.json" << std::endl;
      if(!Generators::ConfigJson::writeConfig(inter->pro)){
        std::cout << "Failed to write config.json" << std::endl;
      }

      if(!Generators::CMakeList::createCMakeListsExecutable(inter->pro)){
        std::cout << "Failed to write CMakeLists.txt" << std::endl;
      }
    }

    return true;
  }
  bool getModeName(Mode &mode){
    Prompt<std::string> *name = new Prompt<std::string>("Name: ");
    name->Run();
    mode.name = name->Get();
    return true;
  }
  bool buildTypeAdd(Interface* interface){
    std::cout << "Adding mode" << std::endl; 
    Mode mode;
    getModeName(mode);
    interface->pro->modes.push_back(mode);
    
    std::cout << "Writing config.json" << std::endl;
    if(!Generators::ConfigJson::writeConfig(interface->pro)){
      std::cout << "Failed to write config.json" << std::endl;
    }

    if(!Generators::CMakeList::createCMakeListsExecutable(interface->pro)){
      std::cout << "Failed to write CMakeLists.txt" << std::endl;
    }
    return true;
  }
  bool Interface::add() {
    if (!(args->count("subcommand") > 0)) {
      std::cout << "Usage add:" << ENDL
        "\tdep: adds a dependency" << ENDL
        "\tflag: adds a flag" << ENDL
        "\tlib:  adds a library" << std::endl;
      return false;
    }
    std::string subcommand = args->operator[]("subcommand").as<std::string>();
    if (subcommand == "packages" || subcommand == "p") {
      OptionsInit::Dependencies(this);
      addDependency(this);
    }

    if (subcommand == "flag") {
      addFlag(this);
    }
    if (subcommand == "modes") {
      OptionsInit::Modes(this);
      buildTypeAdd(this);
    }
    if (subcommand ==  "mode"){
      OptionsInit::Mode(this);
      this->mode();
    }
    return true;
  }
}
