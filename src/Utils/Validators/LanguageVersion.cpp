#include "Frate/Utils/Validation.hpp"
#include <iostream>
#include <string>
#include <regex>
#include <memory>
#include <Frate/Command.hpp> 
#include <Frate/Generators.hpp> 



namespace Utils::Validation {
  bool CppLanguageVersion(std::string lang_ver){
    std::vector<std::string> valid_cpp_versions = {"98", "03", "1x", "11", "14", "17", "20","2x","23"};
    for(std::string version : valid_cpp_versions) {
      if(lang_ver == version) {
        return true;
      }
    }
    return false;
  }
  bool CLanguageVersion(std::string lang_ver){
    std::vector<std::string> valid_c_versions = {"89", "90", "94", "99", "11","1x", "17","20", "2x"};
    for(std::string version : valid_c_versions) {
      if(lang_ver == version) {
        return true;
      }
    }
    return false;
  }
}
