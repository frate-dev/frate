#include <CMaker/Generators.hpp>
#include <CMaker/Command.hpp>
#include <CMaker/Command/Author.hpp>
#include <CMaker/Command/Flags.hpp>
#include <CMaker/Command/Package.hpp>
#include <CMaker/Command/RemoteServers.hpp>
#include <algorithm>
#include <cxxopts.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
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
  std::vector<Handler> Interface::getAddHandlers(){
    return {
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
            return Flags::add(this);
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
  }
  bool Interface::add() {
    std::vector<Handler> addHandlers = getAddHandlers();
    if(args->count("subcommand")){
      std::string subcommand = args->operator[]("subcommand").as<std::string>();
      for(Handler handler : addHandlers){
        for(std::string alias : handler.aliases){
          if(alias == subcommand){
            return handler.callback();
          }
        }
      }
      std::cout << "Unknown subcommand: " << subcommand << ENDL;
      getHelpString("add", addHandlers);
      return false;
    }else{
      std::cout <<  termcolor::bright_red << "No subcommand given" << termcolor::reset << ENDL;
      getHelpString("add", addHandlers);
    }
    return true;
  }
}
