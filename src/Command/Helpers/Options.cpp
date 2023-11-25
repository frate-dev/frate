#include <Frate/Command.hpp>
#include <cxxopts.hpp>
#include <string>


namespace Command {
  using namespace cxxopts;
  /*
   * Initializes the options for options class
   * This allows us to use options in multiple places
   * @param options the options class to initialize
   */
  bool OptionsInit::Flags(Interface* inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand"});
    inter->options->allow_unrecognised_options().add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("m,mode", "mode to add flags", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage");
    inter->options->help();
    return inter->parse();
  }
  //TODO: add
  //
  bool OptionsInit::Packages(Interface* inter) {
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
  bool OptionsInit::Server(Interface *inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand"});
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage");
    return inter->parse();
  }
  bool OptionsInit::Modes(Interface *inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand", "subsubcommand"});
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage")
      ("subsubcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage");
    return inter->parse();
  }
  bool OptionsInit::Mode(Interface *inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand", "mode", "action",  "arguments"});
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "SubCommand to run", cxxopts::value<std::string>()->default_value("help"))
      ("mode", "mode to modify", cxxopts::value<std::string>())("h,help", "Print usage")
      ("action", "action to perform", cxxopts::value<std::string>())
      ("subaction", "action to perform", cxxopts::value<std::string>())
      ("arguments", "Arguments to pass to subcommand", cxxopts::value<std::vector<std::string>>());
    return inter->parse();
  }
} // namespace Command
