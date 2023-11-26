#include <Frate/Generators.hpp>
#include <Frate/Command.hpp>
#include <Frate/Command/CommandMode.hpp>
#include <Frate/Command/Package.hpp>
#include <Frate/Utils/General.hpp>
#include <Frate/Command/Flags.hpp>
#include <Frate/Command/RemoteServers.hpp>
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
  std::vector<Handler> Interface::getRemoveHandlers(){
    return {
      {
        .aliases = {"packages","p","package"},
        .flags = {"-l,--latest","-m,--mode","-t,--target"},
        .positional_args = {"package,..."},
        .docs = "Remove a package from the project",
        .callback = [this]() {
          Packages::options(this);
          return Packages::remove(this);
        },
      },
      {
        .aliases = {"flag", "f"},
        .flags = {"-m,--mode"},
        .positional_args = {"flag,..."},
        .docs = "Remove a flag from the project",
        .callback = [this]() {
          Flags::options(this);
          return Flags::remove(this);
        },
      }
      
    };
  }
  bool Interface::remove() {

    std::vector<Handler> removeHandlers = getRemoveHandlers();
    std::string subcommand;

    if(args->count("subcommand")){

      subcommand = args->operator[]("subcommand").as<std::string>();

    }else{
      Utils::Error error;
      error << "No subcommand given" << std::endl;

      getHelpString("add", removeHandlers);

      return false;
    }

    return runCommand(subcommand, removeHandlers);


    return true;
  }
} // namespace Comman
