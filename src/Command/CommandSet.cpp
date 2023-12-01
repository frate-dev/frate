#include "Frate/Command.hpp"
#include "Frate/Utils/General.hpp"
#include <Frate/Command/Set.hpp>
#include <Frate/Command/RemoteServers.hpp>
#include <Frate/Command/License.hpp>

namespace Command { 
  bool OptionsInit::Set(Interface *inter) {

    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand", "args"});
    inter->options->allow_unrecognised_options().add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage")
      ("args", "Arguments to pass to subcommand", cxxopts::value<std::string>());
    return inter->parse();
  }
  std::vector<Handler> Interface::getSetHandlers(){
    return {
      Handler{
        .aliases = {"license","lc"},
        .positional_args = {"license"},
        .docs = "Set the project's license",
        .callback = [this]() {
          return License::set(this);
        },
      },
      Handler{
        .aliases = {"name","n"},
        .docs = "Set the project's name",
        .callback = [this]() {
          Set::name(this);
          return true;//Name::set(this);
        },
      },
      Handler{
        .aliases = {"version","ver","v"},
        .docs = "Set the project's version",
        .callback = [this]() {
          (void)this;
          return true;//Version::set(this);
        },
        .implemented = false
      },
      Handler{
        .aliases = {"keywords","kw"},
        .docs = "Set the project's keywords",
        .callback = [this]() {
          (void)this;
          return true; //Keywords:set(this);
        },
        .implemented = false
      },
      Handler{
        .aliases = {"server","s"},
        .docs = "Set the project's build server",
        .callback = [this]() {
          (void)this;
          RemoteServers::set(this);
          return true; //Server::set(this);
        },
      }
    };
  }
  bool Interface::set(){
    OptionsInit::Set(this);
    std::vector<Handler> setHandlers = getSetHandlers();
    std::string subcommand;

    if(args->count("subcommand")){

      subcommand = args->operator[]("subcommand").as<std::string>();

    }else{
      Utils::Error error;
      error << "No subcommand given" << std::endl;

      getHelpString("set", setHandlers);

      return false;
    }

    return runCommand(subcommand, setHandlers);
    return true;
  }
}
