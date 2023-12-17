#include "Frate/LuaAPI.hpp"
#include "Frate/Utils/General.hpp"

std::string Frate::LuaAPI::FrateApi::fetch_text(const std::string &url) {
  return Utils::fetchText(url);
}
