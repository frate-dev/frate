#include "Frate/System/Capability.hpp"
#include "Frate/Utils/General.hpp"
#include <Frate/System/Capabilities.hpp>
#include <regex>

namespace Frate::System {
  std::string get_gcc_compiler_version(const std::filesystem::path &path) {
    std::string gcc_version;
    std::string gcc_version_cmd = path.string() + " --version";
    Utils::CmdOutput command = Frate::Utils::hSystemWithOutput(gcc_version_cmd);
    if (command.return_code == 0) {
      std::regex gcc_version_regex(R"(gcc \(GCC\) ([0-9]+\.[0-9]+\.[0-9]+))");
      std::smatch gcc_version_match;
      if (std::regex_search(
              command.std_out, gcc_version_match, gcc_version_regex)) {
        gcc_version = gcc_version_match[1];
        return gcc_version;
      }
    }
    // If we get here we didn't find a version or command failed
    return gcc_version;
  }

  void Capabilities::get_compilers_capability(const std::filesystem::path &path,
                                              std::string &name) {
    std::string gcc_version = get_gcc_compiler_version(path);
    if (!gcc_version.empty()) {
      compilers[name] = Capability();
      compilers[name].version = gcc_version;
      compilers[name].path = path;
      compilers[name].installed = true;
    }
  }
} // namespace Frate::System
