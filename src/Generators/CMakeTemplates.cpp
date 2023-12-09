#include <filesystem>
#include <fstream>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <nlohmann/json.hpp>
#include <Frate/Command.hpp>
#include <Frate/Utils/General.hpp>
#include <sol/sol.hpp>
#include <git2.h>
#include <inja.hpp>
#include <Frate/LuaAPI.hpp>

namespace Frate::Generators::CMakeList {
  using nlohmann::json;

  bool createCMakeLists(std::shared_ptr<Command::Project> pro){
    std::cout << "Creating CMakeLists.txt" << std::endl;

#ifdef DEBUG
#ifndef TEST
    std::cout << pro->toJson().dump(2) << std::endl;
#endif
#endif
    std::string CPM = Utils::fetchText("https://raw.githubusercontent.com/cpm-cmake/CPM.cmake/v0.38.6/cmake/CPM.cmake");
    std::ofstream CPMFile;
    try{
      if(!std::filesystem::exists(pro->project_path / "cmake"))
        std::filesystem::create_directories(pro->project_path / "cmake");
      CPMFile.open(pro->project_path / "cmake/CPM.cmake");
    }catch(...){
      Utils::debug("Error while opening file: CPM.cmake");
      return false;
    }

    CPMFile << CPM;
    inja::Environment env;
    sol::state lua;
    LuaAPI::registerAPI(lua);

    if(!LuaAPI::registerProject(lua, pro)){
      Utils::debug("Error while registering project");
      return false;
    }

    if(!LuaAPI::registerProjectScripts(env, lua,pro->project_path / "templates/scripts")){
      Utils::debug("Error while registering project scripts");
      return false;
    }

    std::cout << "Rendering CMakeLists.txt" << std::endl;

    std::string CMakeListsExecutable;
    try{
      CMakeListsExecutable = env.render_file(pro->project_path /"templates" / "CMakeLists.txt.inja", pro->toJson());
    }catch(...){
      Utils::debug("Error while rendering CMakeLists.txt");
      return false;
    }

    std::cout << "Writing CMakeLists.txt" << std::endl;
    std::ofstream file;
    std::string file_name = "CMakeLists.txt";
    
    if(std::filesystem::exists(pro->project_path / file_name)){

      std::filesystem::remove(pro->project_path / file_name);

    }


    try{
      file.open(pro->project_path / file_name);
    }catch(...){
      Utils::debug("Error while opening file: " + file_name);
      return false;
    }
    std::cout << CMakeListsExecutable << std::endl;
    file << CMakeListsExecutable;

    file.close();
    return true;
  }
}




