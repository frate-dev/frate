#include <Frate/LuaAPI.hpp>

namespace Frate::LuaAPI {
  sol::table to_table(const nlohmann::json& in_json){
    sol::table out_table = sol::table();
    for(auto& [key,value] : in_json.items()){
      if(value.is_object()){

        sol::table new_table = to_table(value);
        out_table[key] = new_table;
      }else if(value.is_array()){

        sol::table new_table = to_table(value);
        out_table[key] = new_table;
      }else if(value.is_string()){

        out_table[key] = value.get<std::string>();
      }else if(value.is_number_integer()){

        out_table[key] = value.get<int>();
      }else if(value.is_number_float()){

        out_table[key] = value.get<float>();
      }else if(value.is_boolean()){

        out_table[key] = value.get<bool>();
      }else if(value.is_null()){

        out_table[key] = sol::nil;
      }else{

        out_table[key] = sol::nil;
      }
    }
    return out_table;
  }
  bool is_table_array(const sol::table& table){
    for(size_t i = 1; i <= table.size(); i++){
      if(!table[i].valid() || !table[i].is<int>()){
        return false;
      }
    }
    return true;
  }
  nlohmann::json from_table(sol::table in_table){
    nlohmann::json out_json;
    for(auto& [key,value] : in_table.pairs()){
      if(value.is<sol::table>()){

        nlohmann::json new_json = from_table(value.as<sol::table>());
        out_json[key.as<std::string>()] = new_json;

      }else if(is_table_array(value)){

        nlohmann::json new_json = from_table(value.as<sol::table>());
        out_json[key.as<std::string>()] = new_json;

      }else if(value.is<std::string>()){

        out_json[key.as<std::string>()] = value.as<std::string>();

      }else if(value.is<int>()){

        out_json[key.as<std::string>()] = value.as<int>();

      }else if(value.is<float>()){

        out_json[key.as<std::string>()] = value.as<float>();

      }else if(value.is<bool>()){

        out_json[key.as<std::string>()] = value.as<bool>();

      }else if(value.is<sol::nil_t>()){

        out_json[key.as<std::string>()] = nullptr;

      }else{

        out_json[key.as<std::string>()] = nullptr;

      }
    }
    return out_json;
  }
}
