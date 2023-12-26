#include <Frate/Generators.hpp>
#include <Frate/Interface.hpp>
#include <regex>
#include <string>

namespace Frate::Utils::Validation {
  bool ProjectVersion(std::string project_version) {
    // Checking if the version is x.x.x
    if (std::regex_match(project_version,
                         std::regex("^[0-9]+\\.[0-9]+\\.[0-9]+$"))) {
      return true;
    }
    // Checking if the version is x.x
    if (std::regex_match(project_version, std::regex("^[0-9]+\\.[0-9]+$"))) {
      return true;
    }
    return false;
  }
} // namespace Frate::Utils::Validation
