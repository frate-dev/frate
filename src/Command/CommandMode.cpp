#include "Command.hpp"
#include "../Generators/Generators.hpp"

namespace Command {
  bool addFlags(Interface* inter, std::string mode){
    std::vector<std::string> flags = inter->args->operator[]("args").as<std::vector<std::string>>();
    for(auto m : inter->pro->modes){
      if(m.name == mode){
        for(std::string f : flags){
          m.flags.push_back(f);
        }
      }
    }
    
    return true;
  }
  bool addDependencies(Interface* inter, std::string mode){

    std::vector<std::string> dependencies = inter->args->operator[]("arguments").as<std::vector<std::string>>();

    for (auto dep : dependencies) {
      Package new_package = getDependency(dep, inter->pro->dependencies);      
      for(Mode &m : inter->pro->modes){
        if(m.name == mode){
          m.dependencies.push_back({
              .name = new_package.name,
              .git = new_package.git,
              .version = new_package.selected_version,
              .target_link = new_package.target_link,
              }
          );
        }
      }
    }
    Generators::ConfigJson::writeConfig(inter->pro);
    Generators::CMakeList::createCMakeListsExecutable(inter->pro);
    return true;
  }
  bool removeDependencies(Interface* inter, std::string mode){
    std::vector<std::string> dependencies = inter->args->operator[]("arguments").as<std::vector<std::string>>();
    for (auto dep : dependencies) {
      for(Mode &m : inter->pro->modes){
        if(m.name == mode){
          for(auto it = m.dependencies.begin(); it != m.dependencies.end(); it++){
            if(it->name == dep){
              m.dependencies.erase(it);
            }
          }
        }
      }
    }
    Generators::ConfigJson::writeConfig(inter->pro);
    Generators::CMakeList::createCMakeListsExecutable(inter->pro);
    return true;
  }
  bool Interface::mode(){
    std::cout << "Running mode: " << std::endl;
    std::string mode = args->operator[]("mode").as<std::string>();
    for(auto m : pro->modes){
      if(m.name == mode){
        std::cout << "Found mode: " << mode << std::endl;
        // are we gonna get some action???
        if(args->count("action")){
          std::string command = args->operator[]("command").as<std::string>();
          std::string action = args->operator[]("action").as<std::string>();
          if(command == "add"){
            if(action == "flags"){
              return addFlags(this, mode);
            }
            if(action == "deps"){
              return addDependencies(this, mode);
            }
          }
          if(command == "remove"){
            if(action == "deps"){
              return removeDependencies(this, mode);
            }
          }
        }
      }
    }
    std::cout << "Failed to find mode: " << mode << std::endl;
    std::cout << "\tmodes found:" << std::endl;
    for(auto m : pro->modes){
      std::cout << "\t\t" << m.name << std::endl;
    }
    return true;
  }
};
