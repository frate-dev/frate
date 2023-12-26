#include "Frate/Utils/Validation.hpp"
#include <Frate/Constants.hpp>
#include <Frate/Generators.hpp>
#include <Frate/Interface.hpp>
#include <string>

namespace Frate::Utils::Validation {
  bool Language(std::string lang) {
    for (std::string supportedLang : Constants::SUPPORTED_LANGUAGES) {
      if (lang == supportedLang) {
        return true;
      }
    }
    return false;
  }
} // namespace Frate::Utils::Validation
