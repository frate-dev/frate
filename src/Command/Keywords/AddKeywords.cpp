#include <Frate/Command/Keywords.hpp>


namespace Command::Keywords{
  bool add(Interface* inter){
    if(!inter->args->count("keywords")){
      std::cout << "No keyword specified" << std::endl;
      return false;
    }
    std::vector<std::string> keywords =
      inter->args->operator[]("keywords").as<std::vector<std::string>>();
    for(auto& keyword : keywords){
      inter->pro->keywords.push_back(keyword);
    }
    if(!inter->pro->writeConfig()){
      return false;
    }
    return true;
  }
}
