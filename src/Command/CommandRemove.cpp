#include <CMaker/Generators.hpp>
#include <CMaker/Command.hpp>
#include <CMaker/Command/CommandMode.hpp>
#include <CMaker/Command/Package.hpp>
#include <CMaker/Command/RemoteServers.hpp>
namespace Command {

  bool OptionsInit::Remove(Interface *inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand", "args"});
    inter->options->allow_unrecognised_options().add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("m,mode", "make changes to compile mode", cxxopts::value<std::string>())
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage")
      ("args", "Arguments to pass to subcommand", cxxopts::value<std::vector<std::string>>());
    return inter->parse();
  }
 
  bool removeFlag(Interface *inter) {
    if (inter->args->count("args") == 0) {
    }
    return true;
  }
  bool Interface::remove() {
    args->count("subcommand");
    if (args->count("subcommand") != 0) {
      std::string subcommand = args->operator[]("subcommand").as<std::string>();
      for (auto arg : args->operator[]("args").as<std::vector<std::string>>()) {
        std::cout << arg << std::endl;
      }
      if (subcommand == "package" || subcommand == "p") {
        Packages::remove(this);
      }
      else if (subcommand == "mode") {

        OptionsInit::Mode(this);
        ModeCommands::removePackages(this, this->args->operator[]("mode").as<std::string>());
        // this->mode();
      }else if(subcommand == "lib"){
        //TODO: remove lib, we don't have a lib in the project sruct
      }else if(subcommand == "flag"){
        //TODO: remove flag
      }else if(subcommand == "toolchain"){
        //TODO: remove toolchain
      }else if(subcommand == "author"){
        //TODO: remove author
      }
      else if (subcommand == "server") {
        RemoteServers::remove(this);
      }
      else if(subcommand == "license"){
        //TODO: remove license
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
} // namespace Comman
