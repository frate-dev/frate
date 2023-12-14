#include <Frate/LuaAPI.hpp>
#include <Frate/Utils/General.hpp>


sol::table Frate::LuaAPI::FrateApi::fetch_json(const std::string &url, sol::this_state s) {
  lua_State *L = s;
  sol::state_view lua(L);
  return to_table(
      Utils::fetchJson(url),lua
      );
}
