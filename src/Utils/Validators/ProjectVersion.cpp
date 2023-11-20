
#include <iostream>
#include <string>
#include <regex>
#include <memory>
#include <Frate/Command.hpp> 
#include <Frate/Generators.hpp> 



namespace Generators::ConfigJson{
  /*
   * Validates the project version
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_json: the config json context
   * @return: true if the project version is valid
   */
  bool validateProjectVersion(std::string prefix,  std::shared_ptr<Command::Project> pro, std::shared_ptr<Config> config_json) {
    std::cout << prefix;
#ifndef TEST
      std::getline(std::cin, config_json->project_version);
#endif
    if(config_json->project_version == "") {
      goto end;
    }
    //Checking if the version is x.x.x 
    if(std::regex_match(config_json->project_version, std::regex("^[0-9]+\\.[0-9]+\\.[0-9]+$"))) {
      goto end;
    }
    //Checking if the version is x.x
    if(std::regex_match(config_json->project_version, std::regex("^[0-9]+\\.[0-9]+$"))) {
      goto end;
    }
    return false;
    //If the version is empty we're gonna set it
    end:
      pro->project_version = config_json->project_version == "" ? pro->project_version : config_json->project_version;
    
    return true;

  }
}

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
