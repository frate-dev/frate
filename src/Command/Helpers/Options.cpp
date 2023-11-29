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
  //TODO: add
  //

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
