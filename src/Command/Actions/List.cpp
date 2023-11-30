#include <Frate/Command/Actions/List.hpp>
#include <Frate/Command/License.hpp>
#include <Frate/Command/Modes.hpp>
#include <Frate/Command/Package.hpp>
#include <Frate/Command/RemoteServers.hpp>
#include <Frate/Command/Toolchains.hpp>

namespace Command::List{
  bool options(Interface* inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand"});
    inter->options->allow_unrecognised_options().add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage");
    inter->options->help();
    return inter->parse();
  }
  
  std::vector<Handler> handlers(Interface *inter) {
    return {
      Handler{
        .aliases = 
        {"modes","m"},
        .docs = "List modes",
        .callback = [inter]() {
          return Modes::list(inter);
        },
        .requires_project = true
      },
      Handler{
        .aliases = 
        {"servers","remote-servers"},
        .docs = "List remote servers",
        .callback = [inter]() {
          return RemoteServers::list(inter);
        }
      },
      Handler{
        .aliases = {"installed-packages","packages","p"},
        .flags = {"-m","--mode"},
        .docs = "List installed packages",
        .callback = [inter]() {
          return Packages::list(inter);
        },
        .requires_project = true
      },
      Handler{
        .aliases = 
        {"flags","f"},
        .docs = "List flags",
        .callback = [inter]() {
          //TODO: List flags
          (void)inter;
          return true;
        },
        .implemented = false,
        .requires_project = true
      },
      Handler{
        .aliases = 
        {"authors","a"},
        .docs = "List authors",
        .callback = [inter]() {
          //TODO: List authors
          (void)inter;
          return true;
        },
        .implemented = false,
        .requires_project = true
      },
      Handler{
        .aliases = 
        {"available-targets","at"},
        .docs = "List available targets",
        .callback = [inter]() {
          (void)inter;
          Command::Toolchains::list();
          return true;
        }
      },
      Handler{
        .aliases = 
        {"licenses","lc"},
        .docs = "List possible licenses",
        .callback = [inter]() {
          return License::list(inter);
        },
        .requires_project = true
      },
    };
  }


  bool run(Interface* inter){
    options(inter);
    std::vector<Handler> listHandlers = handlers(inter);
    std::string target;
    if(inter->args->operator[]("subcommand").count() > 0){
      target = inter->args->operator[]("subcommand").as<std::string>();
    }else{
      std::cout << termcolor::bright_red << "No subcommand provided" << ENDL;
      inter->getHelpString("list", listHandlers);
      return false;
    }
    return inter->runCommand(target, listHandlers);
  }
}
