#include <Frate/LuaAPI.hpp>
#include <filesystem>

namespace Frate::LuaAPI {
  std::string FrateApi::get_path() {
    return std::filesystem::current_path().string()
#ifdef DEBUG
           + "/build"
#endif
        ;
  }
} // namespace Frate::LuaAPI
