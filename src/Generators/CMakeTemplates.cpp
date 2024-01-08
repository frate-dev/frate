#include <Frate/LuaAPI.hpp>
#include <Frate/Project/Config.hpp>
#include <Frate/Utils/General.hpp>
#include <filesystem>
#include <fstream>
#include <inja.hpp>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <sol/sol.hpp>
#include <string>

namespace Frate::Generators::CMakeList {
  using nlohmann::json;

  bool createCMakeLists(std::shared_ptr<Project::Config> pro) {
    std::cout << "Creating CMakeLists.txt" << std::endl;

#ifdef DEBUG
#ifndef TEST
    std::cout << json(*pro).dump(2) << std::endl;
#endif
#endif
    std::string CPM =
        Utils::fetchText("https://raw.githubusercontent.com/cpm-cmake/"
                         "CPM.cmake/v0.38.6/cmake/CPM.cmake");
    std::ofstream CPMFile;
    try {
      if (!std::filesystem::exists(pro->path / "cmake"))
        std::filesystem::create_directories(pro->path / "cmake");
      CPMFile.open(pro->path / "cmake/CPM.cmake");
    } catch (...) {
      Utils::debug("Error while opening file: CPM.cmake");
      return false;
    }

    CPMFile << CPM;
    inja::Environment env;
    sol::state lua;
    LuaAPI::registerAPI(lua);

    if (!LuaAPI::registerProject(lua, pro)) {
      Utils::debug("Error while registering project");
      return false;
    }

    if (!LuaAPI::registerProjectScripts(env, lua,
                                        pro->path / "templates/scripts", pro)) {
      Utils::debug("Error while registering project scripts");
      return false;
    }

    std::cout << "Rendering CMakeLists.txt" << std::endl;

    std::string CMakeListsExecutable;
    try {
      CMakeListsExecutable = env.render_file(
          pro->path / "templates" / "CMakeLists.txt.inja", *pro);
    } catch (...) {
      Utils::debug("Error while rendering CMakeLists.txt");
      return false;
    }

    std::cout << "Writing CMakeLists.txt" << std::endl;
    std::ofstream file;
    std::string file_name = "CMakeLists.txt";

    if (std::filesystem::exists(pro->path / file_name)) {

      std::filesystem::remove(pro->path / file_name);
    }

    try {
      file.open(pro->path / file_name);
    } catch (...) {
      Utils::debug("Error while opening file: " + file_name);
      return false;
    }
    std::cout << CMakeListsExecutable << std::endl;
    file << CMakeListsExecutable;

    file.close();
    return true;
  }
} // namespace Frate::Generators::CMakeList
