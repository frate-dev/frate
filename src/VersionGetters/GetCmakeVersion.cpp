#include <Frate/System.hpp>
#include <Frate/Utils/General.hpp>
#include <regex>

namespace Frate::System {
  std::string getCmakeVersion(std::filesystem::path cmake_path){
    Utils::CmdOutput cmd_output = Utils::hSystemWithOutput(cmake_path.string()+" --version");
    std::regex cmake_version_pattern("cmake version ([0-9]+\\.[0-9]+\\.[0-9]+)");
    std::smatch cmake_version_match;
    std::regex_search(cmd_output.std_out, cmake_version_match, cmake_version_pattern);
  }
}
