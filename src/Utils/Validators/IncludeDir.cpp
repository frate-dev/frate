
#include "Frate/Utils/Validation.hpp"
#include <string>
#include <regex>
#include <Frate/Interface.hpp> 
#include <Frate/Generators.hpp> 

namespace Frate::Utils::Validation {
  bool IncludeDir(std::string include_dir){
    if (include_dir.size() > 255) {
      return false;
    }

    //check if the include directory is valid
    if (std::regex_match(include_dir, std::regex("^[a-zA-Z0-9_-]+$"))) {
      return true;
    }

    return false;
  }
}
