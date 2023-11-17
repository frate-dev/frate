#include "CMaker/Generators/Generators.hpp"
#include "CMaker/Command.hpp"
#include "Author/Author.hpp"
#include <algorithm>
#include <cxxopts.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include "Package/Package.hpp"
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
      Packages::add(this);
    }
    else if (subcommand == "flag") {
      addFlag(this);
    }
    else if (subcommand == "modes") {
      OptionsInit::Modes(this);
      buildTypeAdd(this);
    }
    else if (subcommand == "author") {
      Author::add(this);
    }

    return true;
  }
}
