#include "Frate/Lua/LuaAPI.hpp"
#include "Frate/Utils/General.hpp"

std::string Frate::Lua::FrateApi::fetch_text(const std::string &url) {
  return Utils::fetchText(url);
}
