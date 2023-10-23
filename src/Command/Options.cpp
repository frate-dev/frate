
#include <string>
#include <cxxopts.hpp>
#include "Command.hpp"

namespace Command{
  void initOptions(cxxopts::Options& options){
    std::cout << "fucking options" << ENDL;
    options.parse_positional({"command",  "subcommand"});
    options.add_options()
      ("p,production", "Enable debug mode", cxxopts::value<bool>()->default_value("false"))
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value< std::vector<std::string>>())
      ("h,help", "Print usage")
      ("y,skip-init", "skip init", cxxopts::value<bool>()->default_value("false"))
      ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"));
  }
}
