#include <iostream>
#include <string>
#include <regex>
#include <memory>
#include "../../Command/Command.hpp"
#include "../Generators.hpp"


namespace Generators::ConfigJson{
  /*
   * Validates the cmake version
   * @param prefix: the prefix of the message @param ctx: the context of the
   * command
   * @param config_json: the config json context
   * @return: true if the version is valid
   */
  bool validateCmakeVersion(std::string prefix, std::shared_ptr<Command::Context> ctx, std::shared_ptr<Config> config_json) {
    std::cout << prefix;
#ifndef TEST
    std::getline(std::cin, config_json->cmake_version);
#endif
    //If the version is empty we're gonna set it
    if(config_json->cmake_version == "") {
      goto end;
    }
    //Checking if the version is x.x.x 
    if(std::regex_match(config_json->cmake_version, std::regex("^[0-4]+\\.[0-9]+\\.[0-9]+$"))) {
      goto end;
    }
    //Checking if the version is x.x
    if(std::regex_match(config_json->cmake_version, std::regex("^[0-4]+\\.[0-9]+$"))) {
      goto end;
    }
    return false;
  end:
    ctx->cmake_version = config_json->cmake_version == "" ? ctx->cmake_version : config_json->cmake_version;

    return true;
  }

}
