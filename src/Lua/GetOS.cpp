#include <Frate/Constants.hpp>
#include <Frate/LuaAPI.hpp>

namespace Frate::LuaAPI {
  using std::filesystem::path;

  std::string FrateApi::get_os() { return Frate::Constants::BUILD_OS; }
} // namespace Frate::LuaAPI
