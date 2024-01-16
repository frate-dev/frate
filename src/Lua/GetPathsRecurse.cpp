#include "Frate/Utils/Logging.hpp"
#include <Frate/Lua/LuaAPI.hpp>
#include <filesystem>

namespace Frate::Lua {
  std::vector<std::string> FrateApi::get_paths_recurse(std::string input_path) {
    std::filesystem::path deepest_path = std::filesystem::current_path();
    Utils::info << "Getting paths from " << input_path << std::endl;
    // check if path is absolute
    if (input_path[0] != '/') {
      Utils::error
          << "Frate Lua Api Error: Path in get_paths_recurse must be absolute"
          << std::endl;
      exit(1);
    }

    if (input_path.find(deepest_path.string()) == std::string::npos) {
      Utils::error << "Frate Lua Api Error: Path in get_paths_recurse must be "
                      "in project directory"
                   << std::endl;
      exit(1);
    }

    std::vector<std::string> paths;
    for (const auto &current_path :
         std::filesystem::recursive_directory_iterator(input_path)) {
      paths.emplace_back(current_path.path().string());
    }

    return paths;
  }
} // namespace Frate::Lua
