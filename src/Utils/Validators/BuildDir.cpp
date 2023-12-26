#include "Frate/Utils/Validation.hpp"
#include <Frate/Generators.hpp>
#include <Frate/Interface.hpp>
#include <regex>
#include <string>

namespace Frate::Utils::Validation {
  bool BuildDir(std::string build_dir) {
    if (build_dir.size() > 255) {
      return false;
    }
    // check if the build directory is valid
    // format: name-something_420-69
    if (!std::regex_match(build_dir, std::regex("^[a-zA-Z0-9_-]+$"))) {
      return false;
    }
    return true;
  }
} // namespace Frate::Utils::Validation
