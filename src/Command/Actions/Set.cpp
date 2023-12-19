#include "Frate/Utils/General.hpp"
#include <Frate/Command/Actions/Set.hpp>
#include <Frate/Command/Flags.hpp>
#include <Frate/Command/License.hpp>
#include <Frate/Command/Name.hpp>
#include <Frate/Command/RemoteServers.hpp>
#include <Frate/Generators.hpp>
namespace Frate::Command::Set {

  bool options(std::shared_ptr<Interface> inter) {

    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand", "args"});
    inter->options->allow_unrecognised_options().add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage")
      ("args", "Arguments to pass to subcommand", cxxopts::value<std::string>());
    return inter->parse();
  }
  std::vector<Handler> handlers(std::shared_ptr<Interface> inter) {
    return {
      Handler{
        .aliases = {"license","lc"},
        .positional_args = {"license"},
        .docs = "Set the project's license",
        .callback = &License::set,
        .requires_project = true,
      },
      Handler{
        .aliases = {"name","n"},
        .docs = "Set the project's name",
        .callback = &Name::set,
        .requires_project = true,
      },
      Handler{
        .aliases = {"version","ver","v"},
        .docs = "Set the project's version",
//         .callback = [inter]() {
// 
//           return true;//Version::set(inter);
//         },
        .implemented = false,
        .requires_project = true,
      },
      Handler{
        .aliases = {"server","s"},
        .docs = "Set the project's build server",
        .callback = &RemoteServers::set,
        .requires_project = true,
      }
    };
  }
  bool run(std::shared_ptr<Interface> inter) {
    options(inter);
    std::vector<Handler> setHandlers = handlers(inter);
    std::string subcommand;

    if(inter->args->count("subcommand")){

      subcommand = inter->args->operator[]("subcommand").as<std::string>();

    }else{
      Utils::error << "No subcommand given" << std::endl;

      inter->getHelpString("set", setHandlers);

      return false;
    }

    inter->loadProjectJson();
    Generators::Project::refresh(inter->pro);
    return runCommand(inter,subcommand, setHandlers);
  }
}
