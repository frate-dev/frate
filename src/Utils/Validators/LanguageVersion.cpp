#include "Frate/Utils/Validation.hpp"
#include <iostream>
#include <string>
#include <regex>
#include <memory>
#include <Frate/Command.hpp> 
#include <Frate/Generators.hpp> 
#include <Frate/Constants.hpp>



namespace Utils::Validation {
  bool CppLanguageVersion(std::string lang_ver){
    for(std::string version : Constants::SUPPORTED_CXX_STANDARDS) {
      if(lang_ver == version) {
        return true;
      }
    }
    return false;
  }
  bool CLanguageVersion(std::string lang_ver){
    for(std::string version : Constants::SUPPORTED_C_STANDARDS) {
      if(lang_ver == version) {
        return true;
      }
    }
    return false;
  }
}
