
#include <iostream>
#include <string>
#include <regex>
#include <memory>
#include "../../Command/Command.hpp"
#include "../Generators.hpp"


namespace Generators::ConfigJson{
  /*
   * Validates the source directory
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_json: the config json context
   * @return: true if the source directory is valid
   */
  bool validateSourceDir(std::string prefix, std::shared_ptr<Command::Context> ctx, std::shared_ptr<Config> config_json) {
    std::cout << prefix;
#ifndef TEST
      std::getline(std::cin, config_json->src_dir);
#endif
    //If the source directory is empty we're gonna set it
    if(config_json->src_dir == "") {
      goto end;
    }
    if (config_json->src_dir.size() > 255) {
      return false;
    }
    //check if the source directory is valid
    if (std::regex_match(config_json->src_dir, std::regex("^[a-zA-Z0-9_-]+$"))) {
      goto end;
    }
    if (config_json->src_dir == "src") {
      goto end;
    }
    if (sizeof(config_json->src_dir) > 255) {
      goto end;
    }
    return false;
    end:
      ctx->src_dir = config_json->src_dir == "" ? ctx->src_dir : config_json->src_dir;
    
    return true;
  }
}
