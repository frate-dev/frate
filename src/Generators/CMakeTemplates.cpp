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

    std::unordered_map<std::string, std::string> scripts;


    for(auto& p: std::filesystem::directory_iterator(
          pro->project_path / "templates/scripts"
          )){
      if(p.path().extension() == ".lua"){
        std::string path = p.path().filename().string();
        path = path.substr(0, path.find(".lua"));
        try{
          std::ifstream file(p.path());
          std::string script_str;
          std::string line;
          while(std::getline(file, line)){
            script_str += line + "\n";
          }
          scripts[path] = script_str;
        }catch(...){
          Utils::debug("Error while opening file: " + p.path().string());
          return false;
        }
      }
    }



    lua.open_libraries(sol::lib::base, sol::lib::package);
    lua["project"] = pro;
    lua.new_usertype<Command::Package>("Package",
        "new", sol::no_constructor,
        "name", &Command::Package::name,
        "git", &Command::Package::git,
        "git_short", &Command::Package::git_short,
        "git_prefixed", &Command::Package::git_prefixed,
        "selected_version", &Command::Package::selected_version,
        "versions", &Command::Package::versions,
        "target_link", &Command::Package::target_link,
        "description", &Command::Package::description,
        "selected_version", &Command::Package::selected_version,
        "git_description", &Command::Package::git_description,
        "language", &Command::Package::language,
        "license", &Command::Package::license,
        "owner", &Command::Package::owner,
        "owner_type", &Command::Package::owner_type,
        "stars", &Command::Package::stars,
        "forks", &Command::Package::forks,
        "open_issues", &Command::Package::open_issues,
        "watchers", &Command::Package::watchers
    ); 

    lua.new_usertype<Command::Mode>("Mode",
        "new", sol::no_constructor,
        "name", &Command::Mode::name,
        "flags", &Command::Mode::flags,
        "dependencies", &Command::Mode::dependencies
    );

    lua.new_usertype<Command::Project>("Project"
        "new", sol::no_constructor,
        "name", &Command::Project::project_name,
        "version", &Command::Project::project_version,
        "description", &Command::Project::project_description,
        "authors", &Command::Project::authors,
        "dependencies", &Command::Project::dependencies,
        "toolchains", &Command::Project::toolchains,
        "flags", &Command::Project::flags,
        "modes", &Command::Project::modes
    );


    
    lua.set("project", pro);

    for(auto& [key, script_str]: scripts){
      std::cout << "Adding script: " << key << std::endl;
      env.add_callback(key, -1, [&lua, &script_str](inja::Arguments input_args){
          sol::table args_table = lua.create_table();
          for(const nlohmann::json* arg: input_args){
            if(arg->is_string()){
              args_table.add(arg->get<std::string>());
            }else if(arg->is_number()){
              args_table.add(arg->get<int>());
            }else if(arg->is_boolean()){
              args_table.add(arg->get<bool>());
            }else{
              std::cout << "Error while executing lua script" << std::endl;
              exit(1);
            }
          }
          lua.set("args", args_table); 
          auto result = lua.script(script_str);
          if(result.valid()){
          return result;
          }else{
          std::cout << "Error while executing lua script" << std::endl;
          exit(1);
          }
      });
    }

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




