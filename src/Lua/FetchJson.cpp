#include <Frate/LuaAPI.hpp>
#include <Frate/Utils/General.hpp>

sol::table Frate::LuaAPI::FrateApi::fetch_json(const std::string &url,
                                               sol::this_state inner_state) {
  lua_State *lua_state = inner_state;
  sol::state_view lua(lua_state);
  return to_table(Utils::fetchJson(url), lua);
}
