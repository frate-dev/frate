#pragma once
#include <sol/sol.hpp>
#include <nlohmann/json.hpp>
namespace Frate::Lua {

  sol::table to_table(nlohmann::json in_json, sol::state_view &lua);
  nlohmann::json from_table(sol::table in_table);
  bool is_table_array(const sol::table &table);


}
