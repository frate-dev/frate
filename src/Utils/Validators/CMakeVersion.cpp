#include "Frate/Utils/Validation.hpp"
#include <string>
#include <Frate/Interface.hpp> 
#include <Frate/Generators.hpp> 
#include <Frate/Constants.hpp>


namespace Frate::Utils::Validation {
  bool CmakeVersion(std::string cmake_version){
    for(std::string version : Constants::SUPPORTED_CMAKE_VERSIONS){
      if(cmake_version == version){
        return true;
      }
    }
    return false;
  }
} 
