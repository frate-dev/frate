#include <Frate/Generators.hpp>
#include <Frate/Command.hpp>
#include <Frate/Command/Author.hpp>
#include <Frate/Command/Flags.hpp>
#include <Frate/Command/Package.hpp>
#include <Frate/Command/RemoteServers.hpp>
#include <Frate/Command/Toolchains.hpp>
#include <Frate/Command/Modes.hpp>
#include <cxxopts.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <Frate/Utils/General.hpp>
#include <Frate/Utils/CLI.hpp>
#include <termcolor/termcolor.hpp>
#include <git2.h>

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
    Prompt *name_prompt = new Prompt("Name: ");
    name_prompt->Run();
    auto [valid, name] = name_prompt->Get<std::string>();
    if(!valid){
      std::cout << "Invalid name" << std::endl;
      return false;
    }else{
      mode.name = name;
    }
    return true;
  }
  //TODO: probably needs to be moved to a different file
  bool buildTypeAdd(Interface* interface){
    std::cout << "Adding mode" << std::endl; 
    Mode mode;
    getModeName(mode);
    interface->pro->modes.push_back(mode);
    
    std::cout << "Writing frate-project.json" << std::endl;
    if(!Generators::ConfigJson::writeConfig(interface->pro)){
      std::cout << "Failed to write frate-project.json" << std::endl;
    }

    if(!Generators::CMakeList::createCMakeListsExecutable(interface->pro)){
      std::cout << "Failed to write CMakeLists.txt" << std::endl;
    }
    return true;
  }
  std::vector<Handler> Interface::getAddHandlers(){
    return {
      Handler{
        .aliases = {"packages","p","package"},
        .flags = {"-l,--latest","-m,--mode","-t,--target"},
        .positional_args = {"package,..."},
        .docs = "Add a package to the project",
        .callback = [this]() {
          Packages::options(this);
          return Packages::add(this);
        },
      },
        Handler{
          .aliases = {"flag","f"},
          .positional_args = {"\"flag\""},
          .docs = "Add a flag to the project",
          .callback = [this]() {
            Flags::options(this);
            return Flags::add(this);
          },
        },
        Handler{
          .aliases = {"lib","l"},
          .positional_args = {"library-name"},
          .docs = "Add a library to link to your project",
          .callback = [this]() {
            //TODO implement library
            // OptionsInit::Libraries(this);
            // Libraries::add(this);
            (void)this;
            return false;
          },
          .implemented = false,
        },
        Handler{
          .aliases = {"mode","m"},
          .positional_args = {"mode-name"},
          .docs = "Adds a build mode to your project",
          .callback = [this]() {
            Modes::Options(this);
            return Modes::add(this);
          },
        },
        Handler{
          .aliases = {"server","s"},
          //TODO: Don't know what this requires
          .docs = "Add a remote server to your local config that you can later build to",
          .callback = [this]() {
            return RemoteServers::add(this);
          },
        },
        Handler{
          .aliases = {"toolchain","t"},
          .positional_args = {},
          .docs = "Add a crosscompile toolchain to your project",
          .callback = [this]() {
            //What is this????
            Prompt *toolchain_prompt = new Prompt("Toolchain");
            toolchain_prompt->Run();
            auto [valid, toolchain_name] = toolchain_prompt->Get<std::string>();
            if(!valid){
              std::cout << "Invalid toolchain" << std::endl;
              return false;
            }else{
              return Toolchains::add(toolchain_name, this);
            }
          },
        },
        Handler{
          .aliases = {"author","a"},
          .positional_args = {"author-name"},
          .docs = "Add an author to your project",
          .callback = [this]() {
            return Author::add(this);
          },
        },
    };
  }
  bool Interface::add() {
    std::vector<Handler> addHandlers = getAddHandlers();
    std::string subcommand;
    if(args->count("subcommand")){

      subcommand = args->operator[]("subcommand").as<std::string>();

    }else{
      Utils::Error error;
      error << "No subcommand given" << std::endl;

      getHelpString("add", addHandlers);

      return false;
    }

    return runCommand(subcommand, addHandlers);

    return true;
  }
}
