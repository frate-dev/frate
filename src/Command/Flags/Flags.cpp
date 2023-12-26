#include <Frate/Command/Flags.hpp>
#include <Frate/Generators.hpp>

namespace Frate::Command::Flags {
  bool options(std::shared_ptr<Interface> inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand"});
    inter->options->allow_unrecognised_options().add_options()(
        "command",
        "Command to run",
        cxxopts::value<std::string>()->default_value("help"))(
        "m,mode",
        "mode to add flags",
        cxxopts::value<std::string>()->default_value("help"))(
        "subcommand", "Subcommand to run", cxxopts::value<std::string>())(
        "h,help", "Print usage");
    inter->options->help();
    return inter->parse();
  }

  std::vector<std::string> makeFlags(std::shared_ptr<Interface> inter) {
    std::vector<std::string> raw_flags = inter->args->unmatched();
    std::vector<std::string> flags;
    std::string build_flags = "";
    for (std::string flag : raw_flags) {
      if (flag[0] == '-' && flag[1] == '-') {
        build_flags = "-" + build_flags;
        flags.emplace_back(build_flags);
        build_flags = "";
      }
      flag.erase(std::remove(flag.begin(), flag.end(), '-'), flag.end());
      build_flags += flag;
    }
    if (build_flags != "") {
      build_flags = "-" + build_flags;
      flags.emplace_back(build_flags);
    }
    for (std::string &flag : flags) {
      flag.erase(std::remove(flag.begin(), flag.end(), ' '), flag.end());
    }
    return flags;
  }
} // namespace Frate::Command::Flags
