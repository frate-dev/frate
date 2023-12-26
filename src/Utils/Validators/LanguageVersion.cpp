#include "Frate/Utils/Validation.hpp"
#include <Frate/Constants.hpp>
#include <Frate/Generators.hpp>
#include <Frate/Interface.hpp>
#include <string>

namespace Frate::Utils::Validation {
  bool CppLanguageVersion(std::string lang_ver) {
    for (std::string version : Constants::SUPPORTED_CXX_STANDARDS) {
      if (lang_ver == version) {
        return true;
      }
    }
    return false;
  }

  bool CLanguageVersion(std::string lang_ver) {
    for (std::string version : Constants::SUPPORTED_C_STANDARDS) {
      if (lang_ver == version) {
        return true;
      }
    }
    return false;
  }
} // namespace Frate::Utils::Validation
