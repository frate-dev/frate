#include "Frate/Utils/General.hpp"
#include <Frate/Command/Actions/Remove.hpp>
#include <Frate/Command/Flags.hpp>
#include <Frate/Command/Modes.hpp>
#include <Frate/Command/Package.hpp>
#include <Frate/Command/Author.hpp>
namespace Frate::Command::Remove {

  bool options(Interface *inter) {
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
  std::vector<Handler> handlers(Interface* inter){
    return {
      {
        .aliases = {"packages","p","package"},
        .flags = {"-l,--latest","-m,--mode","-t,--target"},
        .positional_args = {"package_name"},
        .docs = "Remove a package from the project",
        .callback = [inter]() {
          Packages::options(inter);
          return Packages::remove(inter);
        },
        .unlimited_args = true,
      },
      {
        .aliases = {"flag", "f"},
        .flags = {"-m,--mode"},
        .positional_args = {"flags"},
        .docs = "Remove a flag from the project",
        .callback = [inter]() {
          Flags::options(inter);
          return Flags::remove(inter);
        },
      },
      {
        .aliases = {"mode", "m"},
        .positional_args = {"mode"},
        .docs = "Remove a mode from the project",
        .callback = [inter]() {
          Modes::options(inter);
          return Modes::remove(inter);
        },
      },
      {
        .aliases = {"author", "auth"},
        .positional_args = {"author_name"},
        .docs = "Remove an author from the project",
        .callback = [inter]() {
          Author::options(inter);
          return Author::remove(inter);
        },
        .unlimited_args = true,
      }
    };
  }
  bool run(Interface* inter){ 
    options(inter);
    std::vector<Handler> removeHandlers = handlers(inter);
    std::string subcommand;

    if(inter->args->count("subcommand")){

      subcommand = inter->args->operator[]("subcommand").as<std::string>();

    }else{
      Frate::error << "No subcommand given" << std::endl;

      inter->getHelpString("remove", removeHandlers);

      return false;
    }

    return inter->runCommand(subcommand, removeHandlers);
  }
} // namespace Comman
