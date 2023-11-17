#include "../Command.hpp"
#include "../../Generators/Generators.hpp"
#include "../Package/Package.hpp"
#include "CommandMode.hpp"

namespace Command::ModeCommands {
  bool addFlags(Interface* inter, std::string mode){
    //std::vector<std::string> flags = inter->args->operator[]("args").as<std::vector<std::string>>();
    std::vector<std::string>  flags = inter->args->unmatched();
    for(Mode m : inter->pro->modes){
      if(m.name == mode){
        for(std::string f : flags){
          m.flags.push_back(f);
        }
      }
    }
    return true;
  }
  //TODO: Implement this
  bool removeFlags(Interface *inter, std::string mode){
    return true;
  }

  bool addPackages(Interface* inter, std::string mode){
    std::vector<std::string> dependencies = inter->args->operator[]("args").as<std::vector<std::string>>();
    for (std::string dep_str : dependencies) {
      Package new_package = Packages::get(dep_str, inter->pro->dependencies);
      for(Mode &m : inter->pro->modes){
        if(m.name == mode){
          m.dependencies.push_back(new_package);
        }
      }
    }
    Generators::ConfigJson::writeConfig(inter->pro);
    Generators::CMakeList::createCMakeListsExecutable(inter->pro);
    return true;
  }
  bool removePackages(Interface* inter, std::string mode){
    std::vector<std::string> dependencies = inter->args->operator[]("args").as<std::vector<std::string>>();
    for (std::string dep : dependencies) {
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
};
