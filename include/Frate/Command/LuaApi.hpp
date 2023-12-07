#pragma once
#include <sol/sol.hpp>
#include <nlohmann/json.hpp>


namespace Frate::Command::LuaAPI{
  void to_json(nlohmann::json& j, const sol::table& obj);
  void from_json(const nlohmann::json& j, sol::table& obj);
}
