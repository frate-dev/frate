#include <CMaker/Command.hpp>
#include <CMaker/Generators.hpp>
#include <CMaker/Command/Package.hpp>
#include <CMaker/Command/CommandMode.hpp>

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
      auto [found,new_package] = Packages::searchWithPrompt(dep_str,false);
      if(found){
        //Push the found package to the dependencies
        for(Mode &m : inter->pro->modes){
          if(m.name == mode){
            m.dependencies.push_back(new_package);
          }
        }
      }else{
        std::cout << "Package " << dep_str << " not found" << std::endl;
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
          std::erase_if(m.dependencies, [&dep](auto &d) {
              return d.name == dep;
          });

        }
      }
    }
    Generators::ConfigJson::writeConfig(inter->pro);
    Generators::CMakeList::createCMakeListsExecutable(inter->pro);
    return true;
  }
};
