#include "Frate/Utils/Validation.hpp"
#include <iostream>
#include <string>
#include <regex>
#include <memory>
#include <Frate/Command.hpp> 
#include <Frate/Generators.hpp> 
#include <Frate/Constants.hpp>


namespace Utils::Validation {
  bool Language(std::string lang){
    for(std::string supportedLang : Constants::SUPPORTED_LANGUAGES){
      if(lang == supportedLang){
        return true;
      }
    }
    return false;
  }
}
