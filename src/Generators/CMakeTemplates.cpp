#include <filesystem>
#include <map>
#include <string>
#include <nlohmann/json.hpp>
#include <Frate/Command.hpp>
#include <Frate/Utils/General.hpp>
#include <sol/sol.hpp>
#include <git2.h>
#include <inja.hpp>

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
    lua.open_libraries(sol::lib::base, sol::lib::package);
    
    auto render = [&env](std::string str, sol::table data){
      json j;
      data.for_each([&j](sol::object key, sol::object value){
        j[key.as<std::string>()] = value.as<std::string>();
      });
      std::cout << j.dump(2) << std::endl;
      std::cout << str << std::endl;
      return env.render(str, j);
    };
    lua["render"] = render;
    lua.script(R"(
        print(render("{{name}}", {name = "Frate"}))
    )");
    //env.add_callback()
    std::string CMakeListsExecutable =  env.render_file(pro->project_path /"templates" /"CMakeLists.tmpl", pro->toJson());
    std::ofstream file;
    std::string file_name = "CMakeLists.txt";

    try{
      remove((pro->project_path / file_name).c_str());
    }catch(...){
      Utils::debug("Error while removing file: " + file_name);
      return false;
    }

    try{
      file.open(pro->project_path / file_name);
    }catch(...){
      Utils::debug("Error while opening file: " + file_name);
      return false;
    }
    //std::cout << CMakeListsExecutable << std::endl;
    file << CMakeListsExecutable;
    return true;
  }
}




