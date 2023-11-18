#include <CMaker/Generators.hpp>
#include <CMaker/Command.hpp>
#include <CMaker/Command/Author.hpp>
#include <algorithm>
#include <cxxopts.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <CMaker/Command/Package.hpp>
#include <CMaker/Command/RemoteServers.hpp>
#include <string>
#include <CMaker/Utils/General.hpp>
#include <CMaker/Utils/CLI.hpp>
#include <cmath>
#include <termcolor/termcolor.hpp>

namespace Command {
  using nlohmann::json;
  using Utils::CLI::ListItem;

  bool addFlags(Interface *inter) {
    std::cout << "Adding flags" << std::endl; 
    std::vector<std::string> raw_flags = inter->args->unmatched();
    std::vector<std::string> flags;
    std::string build_flags = "";
    for (std::string flag : raw_flags) {
      if (flag[0] == '-' && flag[1] == '-') {
        build_flags = "-" + build_flags;
        flags.push_back(build_flags);
        build_flags = "";
      }
      flag.erase(std::remove(flag.begin(), flag.end(), '-'), flag.end());
      build_flags += flag;
    }
    if (build_flags != "") {
      build_flags = "-" + build_flags;
      flags.push_back(build_flags);
    }
    if (inter->args->count("mode") > 0) {
      std::string mode = inter->args->operator[]("mode").as<std::string>();
      for (Mode &m : inter->pro->modes) {
        if (m.name == mode) {
          for (std::string flag : flags) {
            std::cout << "Adding flag: " << flag << std::endl;
            m.flags.push_back(flag);
          }
          std::cout << "Writing config.json" << std::endl;
          Generators::ConfigJson::writeConfig(inter->pro);
          Generators::CMakeList::createCMakeListsExecutable(inter->pro);
          return true;
        }
      }
    }
    
    for (std::string flag : flags) {
      std::cout << "Adding flag: " << flag << std::endl;
      inter->pro->flags.push_back(flag);
    }
    std::cout << "Writing config.json" << std::endl;
    Generators::ConfigJson::writeConfig(inter->pro);
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
        "\tp, package: adds a package to  your project" << ENDL
        "\tflag: adds a flag" << ENDL
        "\tlib:  adds a library" << std::endl;
      return false;
    }
    std::string subcommand = args->operator[]("subcommand").as<std::string>();
    if (subcommand == "packages" || subcommand == "p") {
      OptionsInit::Dependencies(this);
      Packages::add(this);
    }
    else if (subcommand == "flags") {
      OptionsInit::Flags(this);
      addFlags(this);
    }
    else if (subcommand == "modes") {
      OptionsInit::Modes(this);
      buildTypeAdd(this);
    }
    //TODO add Remote server
    else if (subcommand == "server") {
      //TODO implement server
      RemoteServers::add(this);
    }
    //TODO add toolchain
    else if (subcommand == "author") {
      //TODO implement author
      Author::add(this);
    }

    return true;
  }
}
