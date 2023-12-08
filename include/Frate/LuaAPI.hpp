#pragma once
#include <nlohmann/json.hpp>
#include <sol/sol.hpp>




namespace Frate::LuaAPI {


  sol::table to_table(const nlohmann::json& json);
  nlohmann::json to_json(const sol::table& table);

}
