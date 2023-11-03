#include <iostream>
#include <string>
#include <regex>
#include <memory>
#include "../../Command/Command.hpp"
#include "../Generators.hpp"


namespace Generators::ConfigJson{
  /*
   * Validates the language version
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_json: the config json context
   * @return: true if the language version is valid
   */
  bool validateLanguageVersion(std::string prefix, std::shared_ptr<Command::Context> ctx, std::shared_ptr<Config> config_json) {
    std::cout << prefix;
#ifndef TEST
      std::getline(std::cin, config_json->lang_version);
#endif
    std::vector<std::string> valid_c_versions = {"89", "90", "94", "99", "11", "18"};
    std::vector<std::string> valid_cpp_versions = {"98", "03", "11", "14", "17", "20","23"};

    //If the version is empty we're gonna set it
    if(config_json->lang_version == "") {
      goto end;
    }
    for(std::string version : valid_c_versions) {
      if(config_json->lang_version == version) {
        goto end;
        break;
      }
    }
    for(std::string version : valid_cpp_versions) {
      if(config_json->lang_version == version) {
        goto end;
        break;
      }
    }
    return false;
    end:
      ctx->lang_version = config_json->lang_version == "" ? ctx->lang_version : config_json->lang_version;
    
    return true;
  }
}
