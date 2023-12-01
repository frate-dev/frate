#include "Frate/Utils/General.hpp"
#include <Frate/Command/Actions/Set.hpp>
#include <Frate/Command/Flags.hpp>
#include <Frate/Command/License.hpp>
#include <Frate/Command/Name.hpp>
#include <Frate/Command/RemoteServers.hpp>
namespace Command::Set {

  bool options(Interface *inter) {

    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand", "args"});
    inter->options->allow_unrecognised_options().add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage")
      ("args", "Arguments to pass to subcommand", cxxopts::value<std::string>());
    return inter->parse();
  }
  std::vector<Handler> handlers(Interface *inter) {
    return {
      Handler{
        .aliases = {"license","lc"},
        .positional_args = {"license"},
        .docs = "Set the project's license",
        .callback = [inter]() {
          return License::set(inter);
        },
      },
      Handler{
        .aliases = {"name","n"},
        .docs = "Set the project's name",
        .callback = [inter]() {
          Name::set(inter);
          return true;//Name::set(inter);
        },
      },
      Handler{
        .aliases = {"version","ver","v"},
        .docs = "Set the project's version",
        .callback = [inter]() {
          (void)inter;
          return true;//Version::set(inter);
        },
        .implemented = false
      },
      Handler{
        .aliases = {"keywords","kw"},
        .docs = "Set the project's keywords",
        .callback = [inter]() {
          (void)inter;
          return true; //Keywords::set(inter);
        },
        .implemented = false
      },
      Handler{
        .aliases = {"server","s"},
        .docs = "Set the project's build server",
        .callback = [inter]() {
          (void)inter;
          RemoteServers::set(inter);
          return true; //Server::set(inter);
        },
      }
    };
  }
  bool run(Interface *inter) {
    options(inter);
    std::vector<Handler> setHandlers = handlers(inter);
    std::string subcommand;

    if(inter->args->count("subcommand")){

      subcommand = inter->args->operator[]("subcommand").as<std::string>();

    }else{
      Utils::Error error;
      error << "No subcommand given" << std::endl;

      inter->getHelpString("set", setHandlers);

      return false;
    }

    return inter->runCommand(subcommand, setHandlers);
  }
}
