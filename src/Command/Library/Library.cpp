#include <Frate/Command/Library.hpp>

namespace Frate::Command::Library {
  bool options(std::shared_ptr<Interface> inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand", "lib"});
    // clang-format off
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>())
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())
      ("lib", "lib", cxxopts::value<std::vector<std::string>>());
    // clang-format on
    return inter->parse();
    return true;
  }
} // namespace Frate::Command::Library
