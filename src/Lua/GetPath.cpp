#include <Frate/Lua/LuaAPI.hpp>
#include <filesystem>

namespace Frate::Lua {
  std::string FrateApi::get_path() {
    return std::filesystem::current_path().string()
#ifdef DEBUG
           + "/build"
#endif
        ;
  }
} // namespace Frate::Lua
