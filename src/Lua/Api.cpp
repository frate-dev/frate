#include <Frate/Command/LuaApi.hpp>

namespace Frate::Command::LuaAPI{
  void to_json(nlohmann::json& j, const sol::table& obj){
    for(auto& [k, v] : obj){
      if(v.is<sol::table>()){
        std::string key = k.as<std::string>();
        j[key] = nlohmann::json::object();
        to_json(j[key], v.as<sol::table>());
      }
      if(v.is<std::string>()){
        std::string key = k.as<std::string>();
        j[key] = v.as<std::string>();
      }
      if(v.is<int>()){
        std::string key = k.as<std::string>();
        j[key] = v.as<int>();
      }
    }
  }
}
