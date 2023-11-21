
#include <iostream>
#include <string>
#include <regex>
#include <memory>
#include <Frate/Command.hpp> 
#include <Frate/Generators.hpp> 




namespace Utils::Validation {
  bool ProjectVersion(std::string project_version){
    //Checking if the version is x.x.x 
    if(std::regex_match(project_version, std::regex("^[0-9]+\\.[0-9]+\\.[0-9]+$"))) {
      return true;
    }
    //Checking if the version is x.x
    if(std::regex_match(project_version, std::regex("^[0-9]+\\.[0-9]+$"))) {
      return true;
    }
    return false;
  }
}
