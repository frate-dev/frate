#include "Frate/Utils/Validation.hpp"
#include <iostream>
#include <string>
#include <regex>
#include <memory>
#include <Frate/Command.hpp> 
#include <Frate/Generators.hpp> 



namespace Generators::ConfigJson{
  /*
   * Validates the language
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_json: the config json context
   * @return: true if the language is valid
   */
  [[deprecated("Use the new project wizard")]]
  bool validateLang(std::string prefix, std::shared_ptr<Command::Project> pro, std::shared_ptr<Config> config_json) {
    std::vector<std::string> supportedLangs = {"cpp", "c"};
    std::cout << prefix << ENDL;
    std::cout << "  Supported languages: ( ";
    for(std::string lang : supportedLangs){
      std::cout << lang << " ";
    }
    std::cout << "): ";
#ifndef TEST
      std::getline(std::cin, config_json->lang);
#endif
    
    if(config_json->lang == "") {
      goto end;
    }
    for(std::string lang : supportedLangs){
      if(config_json->lang == lang){
        goto end;
      }
    }
    return false;
    end:
      pro->lang = config_json->lang == "" ? pro->lang : config_json->lang;
    return true;
  }

}

namespace Utils::Validation {
  bool Language(std::string lang){
    std::vector<std::string> supportedLangs = {"cpp", "c"};
    for(std::string supportedLang : supportedLangs){
      if(lang == supportedLang){
        return true;
      }
    }
    return false;
  }
}
