#include <Frate/LuaAPI.hpp>
#include <Frate/Constants.hpp>
namespace Frate::LuaAPI {
  using std::filesystem::path;
  std::string FrateApi::get_os() {
    return Frate::Constants::BUILD_OS;
  }
}
