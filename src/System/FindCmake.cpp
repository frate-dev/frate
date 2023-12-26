#include <regex>
#include <string>
#include <Frate/System.hpp>
#include <Frate/System/Capabilities.hpp>
#include <Frate/Utils/General.hpp>

namespace Frate::System {

  std::string getCmakeVersion(std::filesystem::path cmake_path){
    Utils::CmdOutput cmd_output = Utils::hSystemWithOutput(cmake_path.string()+" --version");
    std::regex cmake_version_pattern("cmake version ([0-9]+\\.[0-9]+\\.[0-9]+)");
    std::smatch cmake_version_match;
    std::regex_search(cmd_output.std_out, cmake_version_match, cmake_version_pattern);
    if(cmake_version_match.size() != 2){
      throw std::runtime_error("Could not get cmake version");
    }
    return cmake_version_match[1];
  }

  void Capabilities::get_cmake_capability(const std::filesystem::path &path){
    std::string cmake_version = getCmakeVersion(path);
    if(cmake_version.empty()){
      return;
    }
    cmake.installed = true;
    cmake.version = cmake_version;
    cmake.path = path;
  }
}
