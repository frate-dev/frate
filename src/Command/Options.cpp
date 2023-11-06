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
      ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"))
      ("name", "Name of the project", cxxopts::value<std::string>()->default_value("false"))
      ("language", "Language of the project", cxxopts::value<std::string>()->default_value("cpp"));
    return inter->parse();
  }
  bool OptionsInit::Add(Interface* inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand", "args"});
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage")
      ("y,skip-init", "skip init", cxxopts::value<bool>()->default_value("false"))
      ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"))
      ("args", "Arguments to pass to subcommand", cxxopts::value<std::vector<std::string>>());
    inter->options->help();
    return inter->parse();
  }
  bool OptionsInit::Remove(Interface *inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand", "args"});
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage")
      ("y,skip-init", "skip init", cxxopts::value<bool>()->default_value("false"))
      ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"))
      ("args", "Arguments to pass to subcommand", cxxopts::value<std::vector<std::string>>());
    return inter->parse();
  }
  bool OptionsInit::Update(Interface *interface) {
    interface->InitHeader();
    interface->options->parse_positional({"command", "subcommand"});
    interface->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage")
      ("y,skip-init", "skip init", cxxopts::value<bool>()->default_value("false"))
      ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"));
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
      ("b,remote-build", "Build server to use", cxxopts::value<bool>()->default_value("false"))
      ("c,args", "command to pass to dev", cxxopts::value<std::vector<std::string>>())
      ("y,skip-init", "skip init", cxxopts::value<bool>()->default_value("true"))
      ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"));
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
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage")
      ("y,skip-init", "skip init", cxxopts::value<bool>()->default_value("true"))
      ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"));
    return interface->parse();
  }
} // namespace Command
