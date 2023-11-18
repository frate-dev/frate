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
  
  bool OptionsInit::Add(Interface* inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand"});
    inter->options->allow_unrecognised_options().add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage");
    inter->options->help();
    return inter->parse();
  }
  //TODO: This should be moved to the flags module
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
    Handler add_handlers[] = {
      Handler{
        .aliases = 
        {"package","p"},
        .docs = "Add a package to the project",
        .callback = [this]() {
          OptionsInit::Dependencies(this);
          return Packages::add(this);
        },
      },
      Handler{
        .aliases = 
        {"flag","f"},
        .docs = "Add a flag to the project",
        .callback = [this]() {
          OptionsInit::Flags(this);
          return addFlags(this);
        },
      },
      Handler{
        .aliases = 
        {"lib","l"},
        .docs = "Add a library to link to your project",
        .callback = [this]() {
          //TODO implement library
          // OptionsInit::Libraries(this);
          // Libraries::add(this);
          return true;
        },
      },
      Handler{
        .aliases = 
        {"mode","m"},
        .docs = "Adds a build mode to your project",
        .callback = [this]() {
          OptionsInit::Modes(this);
          return buildTypeAdd(this);
        },
      },
      Handler{
        .aliases = 
        {"server","s"},
        .docs = "Add a remote server to your local config that you can later build to",
        .callback = [this]() {
          //TODO implement server
          return RemoteServers::add(this);
        },
      },
      Handler{
        .aliases = 
        {"author","a"},
        .docs = "Add an author to your project",
        .callback = [this]() {
          return Author::add(this);
        },
      },
    };
    if(args->count("subcommand")){
      std::string subcommand = args->operator[]("subcommand").as<std::string>();
      for(Handler handler : add_handlers){
        for(std::string alias : handler.aliases){
          if(alias == subcommand){
            return handler.callback();
          }
        }
      }
      std::cout << "Unknown subcommand: " << subcommand << ENDL;
      std::cout << "Usage: add" << ENDL;
      for(Handler handler : add_handlers){
        std::cout << "\t" << handler.aliases[0];
      }
      return false;
    }else{
      std::cout << "Usage: add" << ENDL;
      for(Handler handler : add_handlers){
        std::cout << "\t";
        for(std::string alias : handler.aliases){
          std::cout << alias;
          if(alias != handler.aliases.back()){
            std::cout << " , ";
          }
        }
        std::cout << " : " << handler.docs << ENDL;
      }
    }
    return true;
  }
}
