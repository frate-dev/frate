#include <Frate/Command/CommandMode.hpp>
#include <Frate/Command/Package.hpp>
#include <Frate/Generators.hpp>

namespace Command::Packages {

  bool removePackagesByMode(Interface* inter, std::string mode){
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
  bool remove(Interface *inter) {

    if (inter->args->count("args") == 0) {
      std::cout << R"EOF(
Usage remove dep:
   [args]: the dependencies to remove
   cmake remove dep [args] 

        )EOF" << std::endl;
      return false;
    }
    if (inter->args->count("mode") != 0){
      return removePackagesByMode(inter, inter->args->operator[]("mode").as<std::string>());
    }

    std::vector<std::string> name_to_remove = inter->args->operator[]("args").as<std::vector<std::string>>();
    for (std::string name : name_to_remove) {
      std::cout << "name:" << name << std::endl;
    }
    std::cout << "removing dependencies" << std::endl;
    std::erase_if(inter->pro->dependencies, [&name_to_remove](auto &dep) {
        for (std::string name : name_to_remove) {
          if (dep.name == name) {
            return true;
          }
        }
        return false;
    });

    Generators::ConfigJson::writeConfig(inter->pro);
    Generators::CMakeList::createCMakeListsExecutable(inter->pro);
    return true;
  }

}