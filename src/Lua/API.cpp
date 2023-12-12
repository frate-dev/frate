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
  using std::filesystem::path;
  class FrateApi {
    public:
      FrateApi() = default;
      ~FrateApi() = default;
      static std::string get_os();
      static std::vector<std::string> get_paths_recurse(std::string input_path);
      static std::string get_path();
      static std::string format(const std::string &str, sol::variadic_args var_args);
  };
  std::string FrateApi::get_os() {
    return Frate::Constants::BUILD_OS;
  }
  std::string FrateApi::get_path() {
      return std::filesystem::current_path().string()
      #ifdef DEBUG
        + "/build"
      #endif
      ; 
  }
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
  std::string FrateApi::format(const std::string &str, sol::variadic_args var_args) {
    std::vector<std::string> args;
    std::string result;

    for (const auto &arg : var_args) {
      if (arg.is<std::string>()) {
        args.push_back(arg.as<std::string>());
      } else if (arg.is<int>()) {
        args.push_back(std::to_string(arg.as<int>()));
      } else if (arg.is<bool>()) {
        args.push_back(arg.as<bool>() ? "true" : "false");
      } else if (arg.is<float>()) {
        args.push_back(std::to_string(arg.as<float>()));
      } else {
        args.push_back("nil");
      }
    }

    for (size_t i = 0; i < str.size(); i++) {
      if (str[i] == '{') {
        if (str[i + 1] == '{') {
          result += '{';
          i++;
        } else {
          size_t j = i + 1;
          while (j < str.size() && str[j] != '}') {
            j++;
          }
          if (j == str.size()) {
            throw std::runtime_error("Invalid format string");
          }
          std::string index = str.substr(i + 1, j - i - 1);
          if (index == "0") {
            result += args[0];
          } else {
            int idx = std::stoi(index);
            if (idx < 1 || idx > args.size()) {
              throw std::runtime_error("Invalid format string");
            }
            result += args[idx - 1];
          }
          i = j;
        }
      } else {
        result += str[i];
      }
    }

    return result;
  }

  bool initLua(sol::state &lua){

    lua.open_libraries(sol::lib::base, sol::lib::package,sol::lib::string);

    return true;
  }

  bool registerProjectScripts(inja::Environment &env, sol::state &lua, path script_path) {

    std::unordered_map<std::string, std::string> scripts = {};


    for(const std::filesystem::path& p: std::filesystem::recursive_directory_iterator(script_path)){
      // if(p.filename() == "__global__.lua"){
      //   lua.script_file(p.string());
      // }
      if(p.extension() == ".lua"){
        std::string file_name = p.filename();
        std::string full_script_path = p.string();
        //Yoinkin off the lua extension
        file_name = file_name.substr(0, file_name.find(".lua"));

        std::string prefix;

        //Remove the script path
        Utils::replaceKey(full_script_path, script_path.string(), "");

        //Remove the file name
        Utils::replaceKey(full_script_path, file_name + ".lua", "");

        //Remove the first slash
        Utils::replaceKey(full_script_path, "/", ".");

        //Remove the first dot
        full_script_path = full_script_path.substr(1, full_script_path.size());


        prefix = full_script_path;


        scripts[prefix + file_name] = p.string();
      }
    }
    for(auto& [key, script_path]: scripts){
      env.add_callback( key, -1, [&lua, script_path](inja::Arguments input_args){
          sol::table args_table = lua.create_table();
          for(const nlohmann::json* arg: input_args){
          if(arg->is_string()){
          args_table.add(arg->get<std::string>());
          }else if(arg->is_number()){
          args_table.add(arg->get<int>());
          }else if(arg->is_boolean()){
          args_table.add(arg->get<bool>());
          }else{
          error << "Error while executing lua script" << std::endl;
          exit(1);
          }
          }
          lua.set("args", args_table); 
          if(!std::filesystem::exists(script_path)){
          error << "Lua script not found at path: " << script_path << std::endl;
          exit(1);
          }
          auto result = lua.script_file(script_path);
          if(result.valid()){
            return result;
          }else{
            error << "Error while executing lua script" << std::endl;
            exit(1);
          }
      });
    }
    return true;
  }

  bool registerProject(sol::state &lua, std::shared_ptr<Command::Project> pro) {

    lua.set("project", pro);
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
        "modes", &Command::Project::modes,
        "libs", &Command::Project::libs,
        "license", &Command::Project::license,
        "git", &Command::Project::git,
        "cmake_version", &Command::Project::cmake_version,
        "build_dir", &Command::Project::build_dir,
        "src_dir", &Command::Project::src_dir,
        "include_dir", &Command::Project::include_dir,
        "lang_version", &Command::Project::lang_version,
        "lang", &Command::Project::lang,
        "project_type", &Command::Project::project_type,
        "keywords", &Command::Project::keywords,
        "prompts", &Command::Project::prompts
          );

    lua.new_usertype<Command::ProjectPrompt>("ProjectPrompt",
        "value", &Command::ProjectPrompt::value,
        "default", &Command::ProjectPrompt::default_value,
        "getstr", &Command::ProjectPrompt::get<std::string>,
        "getint", &Command::ProjectPrompt::get<int>,
        "getbool", &Command::ProjectPrompt::get<bool>,
        "getfloat", &Command::ProjectPrompt::get<float>
        );

    lua.new_usertype<FrateApi>("frate",
        "new", sol::no_constructor,
        "get_os", &FrateApi::get_os,
        "get_path", &FrateApi::get_path,
        "get_paths_recurse", &FrateApi::get_paths_recurse,
        "format", &FrateApi::format
        );


    return true;
  }

  void registerAPI(sol::state &lua) {
    initLua(lua);
  }
}
