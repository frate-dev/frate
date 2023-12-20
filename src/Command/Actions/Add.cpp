#include <Frate/Command/Actions/Add.hpp>
#include <Frate/Command/Package.hpp>
#include <Frate/Command/Flags.hpp>
#include <Frate/Command/Toolchains.hpp>
#include <Frate/Command/Keywords.hpp>
#include <Frate/Command/Author.hpp>
#include <Frate/Command/Modes.hpp>
#include <Frate/Command/Library.hpp>
#include <Frate/Project.hpp> 
#include <Frate/Command/RemoteServers.hpp>
#include <Frate/Utils/General.hpp>
namespace Frate::Command::Add {
  bool options(std::shared_ptr<Interface> inter){
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand"});
    inter->options->allow_unrecognised_options().add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage"); 
    inter->options->help();
    return inter->parse();
  }
  bool run(std::shared_ptr<Interface> inter){
    options(inter);
    std::vector<Handler> addHandlers = handlers(inter);
    std::string subcommand;
    if(inter->args->count("subcommand") > 0){

      subcommand = inter->args->operator[]("subcommand").as<std::string>();

    }else{
      Utils::error << "No subcommand given" << std::endl;

      inter->getHelpString("add", addHandlers);

      return false;
    }

    inter->pro->load();

    return runCommand(inter,subcommand, addHandlers);

    return true;
  }
  std::vector<Handler> handlers(std::shared_ptr<Interface> inter){
    return {
      Handler{
        .aliases = {"packages","p","package"},
          .flags = {"-l,--latest","-m,--mode","-t,--target"},
          .positional_args = {"package_name"},
          .docs = "Add a package to the project",
          .callback = &Packages::add,
          .unlimited_args = true,
      },
      Handler{
        .aliases = {"flags","f"},
        .positional_args = {"\"flag\""},
        .docs = "Add a flag to the project",
        .callback = &Flags::add,
      },
      Handler{
        .aliases = {"lib","l"},
        .positional_args = {"lib"},
        .docs = "Add a library to link to your project",
        .callback = &Library::add,
      },
      Handler{
        .aliases = {"mode","m"},
        .positional_args = {"mode-name"},
        .docs = "Adds a build mode to your project",
        .callback = &Modes::add,
      },
      Handler{
        .aliases = {"server","s"},
        //TODO: Don't know what inter requires
        .docs = "Add a remote server to your local config that you can later build to",
        .callback = &RemoteServers::add,
      },
      Handler{
        .aliases = {"toolchain","t"},
        .positional_args = {},
        .docs = "Add a crosscompile toolchain to your project",
        .callback = &Toolchains::add,
      },
      Handler{
        .aliases = {"author","a"},
        .positional_args = {"author-name"},
        .docs = "Add an author to your project",
        .callback = &Author::add,
        .unlimited_args = true,
      },
      Handler{
        .aliases = {"keywords","kw"},
        .positional_args = {"keyword"},
        .docs = "Add keywords to your project",
        .callback = &Keywords::add,
        .unlimited_args = true,
      },
    };
  }
}
