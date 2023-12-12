#include <Frate/LuaAPI.hpp>
#include <filesystem>

namespace Frate::LuaAPI {
  std::vector<std::string> FrateApi::get_paths_recurse(std::string input_path) {
    std::filesystem::path deepest_path = std::filesystem::current_path();
    info << "Getting paths from " << input_path << std::endl;
    //check if path is absolute
    if (input_path[0] != '/') {
      error << "Frate Lua Api Error: Path in get_paths_recurse must be absolute" << std::endl;
      exit(1);
    }

    if(input_path.find(deepest_path.string()) == std::string::npos){
      error << "Frate Lua Api Error: Path in get_paths_recurse must be in project directory" << std::endl;
      exit(1);
    }
    

    std::vector<std::string> paths;
    for (const auto &p : std::filesystem::recursive_directory_iterator(input_path)) {
      paths.push_back(p.path().string());
    }

    return paths;
  }
}
