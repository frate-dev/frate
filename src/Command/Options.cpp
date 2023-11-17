#include "Command.hpp"
#include <cxxopts.hpp>
#include <string>


namespace Command {
  using namespace cxxopts;
  /*
   * Initializes the options for options class
   * This allows us to use options in multiple places
   * @param options the options class to initialize
   */
  bool OptionsInit::Main(Interface* inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command"});
    inter->options->allow_unrecognised_options().add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"))
      ("y,yes", "skip all y/n prompts", cxxopts::value<bool>()->default_value("false"))
      ("h,help", "Print usage");

    return inter->parse();
  }

  bool OptionsInit::Search(Interface* inter){
    inter->InitHeader();
    inter->options->parse_positional({"command","query"});

    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("query", "Arguments to pass to the command", cxxopts::value<std::string>())
      ("h,help", "Print usage");
    return inter->parse();
  }


  bool OptionsInit::Init(Interface* inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand"});
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::vector<std::string>>())("h,help", "Print usage")
      ("y,skip-init", "skip init", cxxopts::value<bool>()->default_value("false"))
      ("name", "Name of the project", cxxopts::value<std::string>()->default_value("false"))
      ("t,type", "Type of the project", cxxopts::value<std::string>()->default_value(ProjectType::EXECUTABLE))
      ("l,language", "Language of the project", cxxopts::value<std::string>()->default_value("cpp"));
    return inter->parse();
  }
  bool OptionsInit::Add(Interface* inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand"});
    inter->options->allow_unrecognised_options().add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage");
    inter->options->help();
    return inter->parse();
  }
  //TODO: add
  //
  bool OptionsInit::Dependencies(Interface* inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand", "args"});
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage")
      ("m,mode", "make changes to compile mode", cxxopts::value<std::string>())
      ("e,exact", "Exact package", cxxopts::value<bool>()->default_value("false"))
      ("l,latest", "Latest package", cxxopts::value<bool>()->default_value("false"))
      ("args", "Arguments to pass to subcommand", cxxopts::value<std::vector<std::string>>());
    inter->options->help();
    return inter->parse();
  }
  bool OptionsInit::Remove(Interface *inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand", "args"});
    inter->options->allow_unrecognised_options().add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage")
      ("args", "Arguments to pass to subcommand", cxxopts::value<std::vector<std::string>>());
    return inter->parse();
  }
  bool OptionsInit::Clean(Interface *inter){

    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand", "args"});
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage")
      ("args", "Arguments to pass to subcommand", cxxopts::value<std::vector<std::string>>())
      ("c,cache", "Cleans build and cache", cxxopts::value<bool>()->default_value("false"));
    return inter->parse();
  }
  bool OptionsInit::Update(Interface *interface) {
    interface->InitHeader();
    interface->options->parse_positional({"command", "subcommand"});
    interface->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage");
    return interface->parse();
  }
  bool OptionsInit::Watch(Interface *interface) {
    interface->InitHeader();
    interface->options->parse_positional({"command"});
    interface->options->add_options()
#ifdef DEBUG
      ("o,ours", "watches cmaker source", cxxopts::value<bool>())
#endif
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("r,remote-build", "Build server to use", cxxopts::value<bool>()->default_value("false"))
      ("c,args", "command to pass to dev", cxxopts::value<std::vector<std::string>>());
    return interface->parse();
  }
  bool OptionsInit::Server(Interface *interface) {
    interface->InitHeader();
    interface->options->parse_positional({"command", "subcommand"});
    interface->options->add_options()
#ifdef DEBUG
      ("o,ours", "watches cmaker source", cxxopts::value<bool>())
#endif
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage");
    return interface->parse();
  }
  bool OptionsInit::Modes(Interface *interface) {
    interface->InitHeader();
    interface->options->parse_positional({"command", "subcommand", "subsubcommand"});
    interface->options->add_options()
#ifdef DEBUG
      ("o,ours", "watches cmaker source", cxxopts::value<bool>())
#endif
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage")
      ("subsubcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage");
    return interface->parse();
  }
  bool OptionsInit::Mode(Interface *interface) {
    interface->InitHeader();
    interface->options->parse_positional({"command", "subcommand", "mode", "action",  "arguments"});
    interface->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "SubCommand to run", cxxopts::value<std::string>()->default_value("help"))
      ("mode", "mode to modify", cxxopts::value<std::string>())("h,help", "Print usage")
      ("action", "action to perform", cxxopts::value<std::string>())
      ("subaction", "action to perform", cxxopts::value<std::string>())
      ("arguments", "Arguments to pass to subcommand", cxxopts::value<std::vector<std::string>>());
    return interface->parse();
  }
} // namespace Command
