#include <Frate/Lua/LuaAPI.hpp>
#include <Frate/Lua/Utils.hpp>
#include <Frate/Utils/General.hpp>

sol::table Frate::Lua::FrateApi::fetch_json(const std::string &url,
                                               sol::this_state inner_state) {
  lua_State *lua_state = inner_state;
  sol::state_view lua(lua_state);
  return to_table(Utils::fetchJson(url), lua);
}
