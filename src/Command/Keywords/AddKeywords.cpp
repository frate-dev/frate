#include "Frate/Utils/General.hpp"
#include <Frate/Command/Keywords.hpp>


namespace Command::Keywords{
  bool add(Interface* inter){
    Utils::Error error;
    options(inter);
    if(inter->args->count("keywords")< 1){
      std::cout << "No keyword specified" << std::endl;
      return false;
    }
    std::vector<std::string> keywords =
      inter->args->operator[]("keywords").as<std::vector<std::string>>();
    for(auto& keyword : keywords){
      for(std::string& current_keyword : inter->pro->keywords){
        if(current_keyword == keyword){
          error << "Keyword already exists" << std::endl;
          return false;
        }
      }
      inter->pro->keywords.push_back(keyword);
    }
    if(!inter->pro->writeConfig()){
      return false;
    }
    return true;
  }
}
