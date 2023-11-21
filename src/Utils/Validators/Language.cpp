#include "Frate/Utils/Validation.hpp"
#include <iostream>
#include <string>
#include <regex>
#include <memory>
#include <Frate/Command.hpp> 
#include <Frate/Generators.hpp> 



namespace Utils::Validation {
  bool Language(std::string lang){
    std::vector<std::string> supportedLangs = {"cpp", "c"};
    for(std::string supportedLang : supportedLangs){
      if(lang == supportedLang){
        return true;
      }
    }
    return false;
  }
}
