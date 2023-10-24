
#include <string>
#include <cxxopts.hpp>
#include "Command.hpp"

namespace Command{
  /*
   * Initializes the options for options class
   * This allows us to use options in multiple places
   * @param options the options class to initialize
   */
  void initOptions(cxxopts::Options& options){
    options.parse_positional({"command", "subcommand", "args"});
    options.add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>()->default_value("help"))
      ("args", "Subcommand to run", cxxopts::value< std::vector<std::string>>())
      ("h,help", "Print usage")
      ("y,skip-init", "skip init", cxxopts::value<bool>()->default_value("false"))
      ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"));
  }
}
