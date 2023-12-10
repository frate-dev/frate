#include <Frate/Command/Library.hpp>


namespace Frate::Command::Library{
  bool options(Interface* inter){
    inter->InitHeader();
    inter->options->parse_positional({"command","subcommand","lib"});
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>())
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())
      ("lib", "lib", cxxopts::value<std::vector<std::string>>());
    return inter->parse();
    return true;
  }
}
