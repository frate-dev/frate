#include <Frate/Generators.hpp>
#include <Frate/Interface.hpp>
#include <regex>
#include <string>

namespace Frate::Utils::Validation {
  bool ProjectName(std::string project_name) {
    if ((project_name.size() > 255)) {
      return false;
    }
    if (std::regex_match(project_name, std::regex("^[a-zA-Z0-9_-]+$"))) {
      return true;
    }
    return false;
  }
} // namespace Frate::Utils::Validation
