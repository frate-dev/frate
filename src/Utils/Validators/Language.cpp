#include "Frate/Utils/Validation.hpp"
#include <string>
#include <Frate/Interface.hpp> 
#include <Frate/Generators.hpp> 
#include <Frate/Constants.hpp>


namespace Frate::Utils::Validation {
  bool Language(std::string lang){
    for(std::string supportedLang : Constants::SUPPORTED_LANGUAGES){
      if(lang == supportedLang){
        return true;
      }
    }
    return false;
  }
}
