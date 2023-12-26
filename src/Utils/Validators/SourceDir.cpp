#include <Frate/Generators.hpp>
#include <Frate/Interface.hpp>
#include <regex>
#include <string>

namespace Frate::Utils::Validation {
  bool SourceDir(std::string src_dir) {
    if (src_dir.size() > 255) {
      return false;
    }
    // check if the source directory is valid
    if (std::regex_match(src_dir, std::regex("^[a-zA-Z0-9_-]+$"))) {
      return true;
    }
    if (src_dir == "src") {
      return true;
    }
    if (sizeof(src_dir) > 255) {
      return false;
    }
    return false;
  }
} // namespace Frate::Utils::Validation
