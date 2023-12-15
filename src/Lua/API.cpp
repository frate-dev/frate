#include "Frate/Command.hpp"
#include "Frate/Utils/General.hpp"
#include "inja.hpp"
#include <Frate/LuaAPI.hpp>
#include <filesystem>
#include <memory>
#include <sol/forward.hpp>
#include <sol/variadic_args.hpp>
#include <Frate/Constants.hpp>

namespace Frate::LuaAPI {
using Command::Project;
using std::filesystem::path;

bool registerProjectScripts(inja::Environment &env, sol::state &lua,
                            path script_path, std::shared_ptr<Project> project) {
  std::unordered_map<std::string, std::string> scripts = {};
  
  sol::table global_table = to_table(project->variables,lua);

  lua.set("global", global_table);

  for (const std::filesystem::path &p :
           std::filesystem::recursive_directory_iterator(script_path)) {
    if (p.extension() == ".lua") {
      std::string file_name = p.filename();
      std::string full_script_path = p.string();
      // Yoinkin off the lua extension
      file_name = file_name.substr(0, file_name.find(".lua"));

      std::string prefix;

      // Remove the script path
      Utils::replaceKey(full_script_path, script_path.string(), "");

      // Remove the file name
      Utils::replaceKey(full_script_path, file_name + ".lua", "");

      // Remove the first slash
      Utils::replaceKey(full_script_path, "/", ".");

      // Remove the first dot
      full_script_path = full_script_path.substr(1, full_script_path.size());

      prefix = full_script_path;

      scripts[prefix + file_name] = p.string();
    }
  }


  for (auto &[key, script_path] : scripts) {
    env.add_callback(
        key, -1, [&lua, script_path](inja::Arguments input_args) {
          sol::table args_table = lua.create_table();
          for (const nlohmann::json *arg : input_args) {
            if (arg->is_string()) {
              args_table.add(arg->get<std::string>());
            } else if (arg->is_number()) {
              args_table.add(arg->get<int>());
            } else if (arg->is_boolean()) {
              args_table.add(arg->get<bool>());
            } else {
              error 
              << "Error while converting arguments in inja callback for script at: " 
              << script_path << std::endl;
              exit(1);
            }
          }
          lua.set("args", args_table);

          if (!std::filesystem::exists(script_path)) {
            error << "Lua script not found at path: " << script_path
                  << std::endl;
            exit(1);
          }
          
          info << "Executing lua script at: " << script_path << std::endl;
          //lua.set("global", global_table);
          auto result = lua.script_file(script_path);

          if (result.valid()) {
            return result;
          } else {
            error << "Error while executing lua script at: " << script_path << std::endl;
            exit(1);
          }
        });
  }
  return true;
}

  void registerAPI(sol::state &lua) {
    lua.open_libraries(
        sol::lib::base,
        sol::lib::package,
        sol::lib::string);

    lua.new_usertype<FrateApi>("frate",
        "new", sol::no_constructor,
        "get_os", &FrateApi::get_os,
        "get_path", &FrateApi::get_path,
        "get_paths_recurse", &FrateApi::get_paths_recurse,
        "format", &FrateApi::format,
        "print_table", &FrateApi::print_table,
        "fetch_text", &FrateApi::fetch_text,
        "fetch_json", &FrateApi::fetch_json
        );
  }

  bool initScripts(sol::state &lua, std::shared_ptr<Project> project){
    path script_path = project->path 
      / (Constants::TEMPLATE_PATH + Constants::INIT_SCRIPTS_PATH);

    if(!std::filesystem::exists(script_path)){
      warning << "No init scripts found at: " << script_path << std::endl;
      return false;
    }

    std::vector<path> script_paths = {};

    for(const path& current_path :
        std::filesystem::recursive_directory_iterator(script_path)){
      if(current_path.extension() == ".lua"){
        script_paths.push_back(current_path);
      }
    }
    


    for(const path& current_script_path : script_paths){

      lua.set("project", project);
      if(!std::filesystem::exists(current_script_path)){
        error << "Script not found: " << current_script_path << " at: " << script_path << std::endl;
        return false;
      }
      auto result = lua.script_file(current_script_path);
      if(!result.valid()){
        error << "Error while executing lua script at: " << current_script_path << std::endl;
        return false;
      }

      project = lua.get<std::shared_ptr<Project>>("project");

    }
    return true;
  }

  bool postScripts(sol::state &lua, std::shared_ptr<Project> project){
    path script_path = project->path / 
      (Constants::TEMPLATE_PATH + Constants::POST_SCRIPTS_PATH);

    if(!std::filesystem::exists(script_path)){
      warning << "No post scripts found" << " at: " << script_path << std::endl;
      return false;
    }

    std::vector<path> scripts = {};

    for(const path& current_path :
        std::filesystem::recursive_directory_iterator(script_path)){
      if(current_path.extension() == ".lua"){
        scripts.push_back(current_path);
      }
    }

    for(const path& script : scripts){

      lua.set("project", project);

      if(!std::filesystem::exists(script)){
        error << "Script not found: " << script << " at path: " << script_path << std::endl;
        return false;
      }
      auto result = lua.script_file(script);
      if(!result.valid()){
        error << "Error while executing lua script" << std::endl;
        return false;
      }

      project = lua.get<std::shared_ptr<Project>>("project");
    }
    return true;
  }
}
