
#include <iostream>
#include <string>
#include <regex>
#include <memory>
#include "../../Command/Command.hpp"
#include "../Generators.hpp"


namespace Generators::ConfigJson{
  /*
   * Validates the project version
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_json: the config json context
   * @return: true if the project version is valid
   */
  bool validateProjectVersion(std::string prefix,  std::shared_ptr<Command::Context> ctx, std::shared_ptr<Config> config_json) {
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
      ctx->project_version = config_json->project_version == "" ? ctx->project_version : config_json->project_version;
    
    return true;

  }
}
