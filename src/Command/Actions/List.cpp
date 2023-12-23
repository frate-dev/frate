#include <Frate/Command/Actions/List.hpp>
#include <Frate/Command/License.hpp>
#include <Frate/Command/Modes.hpp>
#include <Frate/Command/Author.hpp>
#include <Frate/Command/Flags.hpp>
#include <Frate/Command/Package.hpp>
#include <Frate/Command/RemoteServers.hpp>
#include <Frate/Command/Toolchains.hpp>
#include "Frate/Command/Library.hpp"

namespace Frate::Command::List{
  bool options(std::shared_ptr<Interface> inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand"});
    inter->options->allow_unrecognised_options().add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage");
    inter->options->help();
    return inter->parse();
  }
  
  std::vector<Handler> handlers(std::shared_ptr<Interface> inter) {
    return {
      Handler{
        .aliases = 
        {"modes","m"},
        .docs = "List modes",
        .callback = &Modes::list,
        .requires_project = true
      },
      Handler{
        .aliases = 
        {"servers","remote-servers"},
        .docs = "List remote servers",
        .callback = &RemoteServers::list,
      },
      Handler{
        .aliases = {"installed-packages","packages","p"},
        .flags = {"-m","--mode"},
        .docs = "List installed packages",
        .callback = &Command::Packages::list,
        .requires_project = true
      },
      Handler{
        .aliases = 
        {"flags","f"},
        .docs = "List flags",
        .callback = &Flags::list,
        .requires_project = true
      },
      Handler{
        .aliases = 
        {"libraries", "lib", "l"},
        .docs = "List flags",
        .callback = &Library::list,
        .requires_project = true
      },
      Handler{
        .aliases = 
        {"authors","a"},
        .docs = "List authors",
        .callback = &Author::list,
        .requires_project = true
      },
      Handler{
        .aliases = 
        {"available-targets","at"},
        .docs = "List available targets",
        .callback = &Toolchains::list,
      },
      Handler{
        .aliases = 
        {"licenses","lc"},
        .docs = "List possible licenses",
        .callback = &License::list,
        .requires_project = true
      },
    };
  }


  bool run(std::shared_ptr<Interface> inter){
    options(inter);
    std::vector<Handler> listHandlers = handlers(inter);
    std::string target;
    if(inter->args->operator[]("subcommand").count() > 0){
      target = inter->args->operator[]("subcommand").as<std::string>();
    }else{
      std::cout << termcolor::bright_red << "No subcommand provided" << std::endl;
      inter->getHelpString("list", listHandlers);
      return false;
    }
    return runCommand(inter,target, listHandlers);
  }
}
