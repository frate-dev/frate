#include <Frate/Command/Keywords.hpp>

namespace Frate::Command::Keywords {
  bool options(std::shared_ptr<Interface> inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand", "keywords"});
    // clang-format off
    inter->options->add_options()
      ("command", "The command to run", cxxopts::value<std::string>())
      ("subcommand", "The subcommand to run", cxxopts::value<std::string>())
      ("keywords","The keywords to search for",cxxopts::value<std::vector<std::string>>());
    // clang-format on
    return inter->parse();
  }
} // namespace Frate::Command::Keywords
