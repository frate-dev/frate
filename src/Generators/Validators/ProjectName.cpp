#include <iostream>
#include <string>
#include <regex>
#include <memory>
#include "../../Command/Command.hpp"
#include "../Generators.hpp"


namespace Generators::ConfigJson{
  /*
   * Validates the project name
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_json: the config json context
   * @return: true if the project name is valid
   */
  bool validateProjectName(std::string prefix, std::shared_ptr<Command::Project> pro, std::shared_ptr<Config> config_json) {
    std::cout << prefix;
    #ifndef TEST
      std::getline(std::cin, config_json->project_name);
    #endif
    //If the name is empty we're gonna set it
    if(config_json->project_name == "") {
      goto end;
    }
    //Checking if the name is valid
    if ((config_json->project_name.size() > 255)) {
      return false;
    }
    if(std::regex_match(config_json->project_name, std::regex("^[a-zA-Z0-9_-]+$"))) {
      goto end;
    }

    return false;
    end:
      pro->project_name = config_json->project_name == "" ? pro->project_name : config_json->project_name;

    return true;
  }
}
