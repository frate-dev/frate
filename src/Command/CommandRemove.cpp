#include "../Generators/Generators.hpp"
#include "Command.hpp"
namespace Command {

  bool removeDep(Interface *inter) {

    if (inter->args->count("args") == 0) {
      std::cout << R"EOF(
Usage remove dep:
   [inter->args]: the dependencies to remove
   cmake remove dep [inter->args] 

        )EOF" << std::endl;
      return false;
    }
    std::vector<std::string> name_to_remove = inter->args->operator[]("inter->args").as<std::vector<std::string>>();
    for (std::string name : name_to_remove) {
      std::cout << "inter->args:" << name << std::endl;
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
  bool Interface::remove() {
    args->count("subcommand");
    if (args->count("subcommand") != 0) {
      std::string subcommand = args->operator[]("subcommand").as<std::string>();
      for (auto arg : args->operator[]("args").as<std::vector<std::string>>()) {
        std::cout << arg << std::endl;
      }
      if (subcommand == "dep") {
        removeDep(this);
      }
      if (subcommand == "mode") {
        OptionsInit::Mode(this);
        this->mode();
      }
    }else{
    std::cout <<  R"EOF(
Usage remove:
  dep: removes dependencies
  lib:  removes libraries
  flag: removes  flags
        )EOF" << std::endl;
    }

    return true;
  }
} // namespace Command
