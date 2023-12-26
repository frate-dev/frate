
#include <Frate/Command/Name.hpp>

namespace Frate::Command::Name {

  bool options(std::shared_ptr<Interface> inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand", "name"});
    inter->options->add_options()(
        "command", "The command to name", cxxopts::value<std::string>())(
        "subcommand", "The subcommand to name", cxxopts::value<std::string>())(
        "name", "The name to give the command", cxxopts::value<std::string>());
    return inter->parse();
  }
} // namespace Frate::Command::Name
