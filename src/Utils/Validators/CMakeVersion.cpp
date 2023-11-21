#include "Frate/Utils/Validation.hpp"
#include <iostream>
#include <string>
#include <regex>
#include <memory>
#include <Frate/Command.hpp> 
#include <Frate/Generators.hpp> 



namespace Utils::Validation {
  bool CmakeVersion(std::string cmake_version){
    //Checking if the version is x.x.x 
    if(std::regex_match(cmake_version, std::regex("^[0-4]+\\.[0-9]+\\.[0-9]+$"))) {
      return true;
    }
    //Checking if the version is x.x
    if(std::regex_match(cmake_version, std::regex("^[0-4]+\\.[0-9]+$"))) {
      return true;
    }
    return false;
  }
} 
