#include <Frate/Constants.hpp>
#include <Frate/Lua/LuaAPI.hpp>

namespace Frate::Lua {
  using std::filesystem::path;

  std::string FrateApi::get_os() { return Frate::Constants::BUILD_OS; }
} // namespace Frate::Lua
