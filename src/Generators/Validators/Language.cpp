#include <iostream>
#include <string>
#include <regex>
#include <memory>
#include "../../Command/Command.hpp"
#include "../Generators.hpp"


namespace Generators::ConfigToml {
  /*
   * Validates the language
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the language is valid
   */
  bool validateLang(std::string prefix, std::shared_ptr<Command::Context> ctx, std::shared_ptr<Config> config_toml) {
    std::vector<std::string> supportedLangs = {"cpp", "c"};
    std::cout << prefix << ENDL;
    std::cout << "  Supported languages: ( ";
    for(std::string lang : supportedLangs){
      std::cout << lang << " ";
    }
    std::cout << "): ";
#ifndef TEST
      std::getline(std::cin, config_toml->lang);
#endif
    
    if(config_toml->lang == "") {
      goto end;
    }
    for(std::string lang : supportedLangs){
      if(config_toml->lang == lang){
        goto end;
      }
    }
    return false;
    end:
      ctx->lang = config_toml->lang == "" ? ctx->lang : config_toml->lang;
    return true;
  }
}
