#include <Frate/LuaAPI.hpp>


namespace Frate::LuaAPI {
  void to_table(sol::table& out_table,const nlohmann::json& json){
    // for(auto& [key,value] : json.items()){
    //   if(value.is_object()){
    //     sol::table new_table = out_table.create_with(key);
    //     to_table(new_table,value);
    //   }else if(value.is_array()){
    //     sol::table new_table = out_table.create_with(key);
    //     to_table(new_table,value);
    //   }else if(value.is_string()){
    //     out_table[key] = value.get<std::string>();
    //   }else if(value.is_number_integer()){
    //     out_table[key] = value.get<int>();
    //   }else if(value.is_number_float()){
    //     out_table[key] = value.get<float>();
    //   }else if(value.is_boolean()){
    //     out_table[key] = value.get<bool>();
    //   }else if(value.is_null()){
    //     out_table[key] = sol::nil;
    //   }
    // }
  }
  bool is_table_array(const sol::table& table){
  //   for(int i = 1; i <= table.size(); i++){
  //     if(!table[i].valid()){
  //       return false;
  //     }
  //   }
  //   return true;
  // }
  // void from_table(nlohmann::json& output_json,const sol::table& table){
  //   for(auto& [raw_key,value] : table){
  //     std::string key = raw_key.as<std::string>();
  //     if(value.is<sol::table>()){
  //       nlohmann::json new_json;
  //       from_table(new_json,value.as<sol::table>());
  //       output_json[key] = new_json;
  //     }else if(is_table_array(value.as<sol::table>())){
  //       nlohmann::json new_json;
  //       from_table(new_json,value.as<sol::table>());
  //       output_json[key] = new_json;
  //     }else if(value.is<std::string>()){
  //       output_json[key] = value.as<std::string>();
  //     }else if(value.is<int>()){
  //       output_json[key] = value.as<int>();
  //     }else if(value.is<float>()){
  //       output_json[key] = value.as<float>();
  //     }else if(value.is<bool>()){
  //       output_json[key] = value.as<bool>();
  //     }else if(value.is<sol::nil_t>()){
  //       output_json[key] = nullptr;
  //     }
  //   }
  }
  
}
