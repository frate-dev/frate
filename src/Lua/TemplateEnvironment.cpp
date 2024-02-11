#include "Frate/Project/Local.hpp"
#include <Frate/Lua/Exceptions.hpp>
#include <Frate/Lua/LuaAPI.hpp>
#include <Frate/Lua/TemplateEnvironment.hpp>
#include <memory>

namespace Frate::Lua {

  void TemplateEnvironment::registerMacroScript(std::string name,
                                                std::string script_text) {
    macro_scripts[name] = script_text;
    register_inja_function(name, script_text);
  };

  void TemplateEnvironment::registerInitScript(std::string name,
                                               std::string script_text) {
    init_scripts[name] = script_text;
  };

  void TemplateEnvironment::registerPostScript(std::string name,
                                               std::string script_text) {
    post_scripts[name] = script_text;
  };

  void TemplateEnvironment::runInitScripts(){
    for(auto &script : init_scripts){
      lua->set("project", pro);
      lua->set("plocal", local);
      Utils::verbose << "Build command: " << local->getBuildCommand() << std::endl;
      auto result = lua->script(script.second);
      if (!result.valid()) {
        throw LuaException("Failed to run init script: " + script.first + result.get<sol::error>().what());
      }
      *pro = lua->get<Project::Config>("project");
      *local = lua->get<Project::Local>("plocal");
      // *local = lua->get<Project::Local>("plocal");
      Utils::verbose << "Build command: " << local->getBuildCommand() << std::endl;
    }
  }

  void TemplateEnvironment::runPostScripts(){
    for(auto &script : post_scripts){
      lua->set("project", pro);
      lua->set("plocal", local);
      auto result = lua->script(script.second);
      if (!result.valid()) {
        throw LuaException("Failed to run post script: " + script.first + result.get<sol::error>().what());
      }
      *pro = lua->get<Project::Config>("project");
      *local = lua->get<Project::Local>("plocal");
    }
  }


  void TemplateEnvironment::templateFile(std::filesystem::path input_file,
                                         std::filesystem::path output_file) {
    Utils::verbose << "Templating file: " << input_file << std::endl;
    if(!std::filesystem::exists(input_file)){
      throw LuaException("Input file does not exist: " + input_file.string());
    }

    if(!std::filesystem::exists(output_file.parent_path())){
      std::filesystem::create_directories(output_file.parent_path());
    }

    if(pro == nullptr){
      throw LuaException("Project is null while templating file: " + input_file.string() + " to " + output_file.string());
    }

    try {

      std::string rendered_file = env.render_file(input_file.string(), *pro);
      std::ofstream output_stream(output_file);
      output_stream << rendered_file;
      output_stream.close();

    } catch (std::exception &e) {

      Utils::error << e.what() << std::endl;
      throw LuaException("Error templating file: " + input_file.string() + " to " + output_file.string());

    }
  }

  std::string TemplateEnvironment::relativePathToNamespace(std::filesystem::path root_path ,std::filesystem::path file_path){
    std::filesystem::path relative_path = std::filesystem::relative(file_path, root_path);
    std::string namespace_str = relative_path.string();
    // Remove the extension
    namespace_str = namespace_str.substr(0, namespace_str.find("."));
    // Replace the slashes with dots
    namespace_str = namespace_str.replace(namespace_str.begin(), namespace_str.end(), "/", ".");
    return namespace_str;
  }

  std::string TemplateEnvironment::relativePathToNamespace(std::filesystem::path file_path){
    std::string namespace_str;
    namespace_str += file_path.string();
    // Remove the extension
    if(file_path.has_extension()){
      namespace_str = namespace_str.substr(0, namespace_str.find("."));
    }
    // Replace the slashes with dots
    std::replace(namespace_str.begin(), namespace_str.end(), '/', '.');

    return namespace_str;
  }

  void TemplateEnvironment::register_frate_api(){
    lua->open_libraries(sol::lib::base, sol::lib::package, sol::lib::string);
    // clang-format off
    lua->new_usertype<FrateApi>(
        "frate", 
        "new", sol::no_constructor,
        "get_os", &FrateApi::get_os,
        "get_path", &FrateApi::get_path,
        "get_paths_recurse", &FrateApi::get_paths_recurse,
        "format", &FrateApi::format,
        "print_table", &FrateApi::print_table,
        "fetch_text", &FrateApi::fetch_text,
        "fetch_json", &FrateApi::fetch_json
        );
    // clang-format on

  };

  void
  TemplateEnvironment::register_inja_function(std::string name,
                                              std::string lua_script_text) {
    // Creating lua and project state so that we can pass them to the callback
    Utils::verbose << "Registering inja function: " << name << std::endl;
    const auto script_callback =
        [lua_script_text, lua_state = this->lua,
         pro_config = this->pro](inja::Arguments &input_args) {

          sol::table args_table = lua_state->create_table();
          for (const nlohmann::json *arg : input_args) {
            if (arg->is_string()) {
              args_table.add(arg->get<std::string>());
            }
            else if (arg->is_number()) {
              args_table.add(arg->get<int>());
            }
            else if (arg->is_boolean()) {
              args_table.add(arg->get<bool>());
            }
            else {
              throw Frate::Lua::LuaException("Invalid argument type");
            }
          }
          lua_state->set("args", args_table);
          lua_state->set("global", pro_config->global);

          auto result = lua_state->script(lua_script_text);

          if (!result.valid()) {
            throw Frate::Lua::LuaException(result.get<sol::error>().what());
          }
          return result;
        };

    env.add_callback(name, script_callback);
  }


  void TemplateEnvironment::register_user_types() {

    lua->set("project", pro);
    //lua->set("plocal", local);

    // clang-format off
    lua->new_usertype<Command::Package>("Package",
        "new",
        sol::no_constructor,
        "name",
        &Command::Package::name,
        "git",
        &Command::Package::git,
        "git_short",
        &Command::Package::git_short,
        "git_prefixed",
        &Command::Package::git_prefixed,
        "versions",
        &Command::Package::versions,
        "target_link",
        &Command::Package::target_link,
        "description",
        &Command::Package::description,
        "git_description",
        &Command::Package::git_description,
        "language",
        &Command::Package::language,
        "license",
        &Command::Package::license,
        "owner",
        &Command::Package::owner,
        "owner_type",
        &Command::Package::owner_type,
        "stars",
        &Command::Package::stars,
        "forks",
        &Command::Package::forks,
        "open_issues",
        &Command::Package::open_issues,
        "watchers",
        &Command::Package::watchers);
    lua->new_usertype<Command::Dependency>("Dependency",
        "new",
        sol::no_constructor,
        "name",
        &Command::Dependency::name,
        "version",
        &Command::Dependency::version,
        "git",
        &Command::Dependency::git,
        "git_short",
        &Command::Dependency::git_short,
        "git_prefixed",
        &Command::Dependency::git_prefixed,
        "target_link", 
        &Command::Dependency::target_link);

    lua->new_usertype<Project::Mode>("Mode",
        "new",
        sol::no_constructor,
        "name",
        &Project::Mode::name,
        "flags",
        &Project::Mode::flags,
        "dependencies",
        &Project::Mode::dependencies);

    lua->new_usertype<Project::Config>("Project"
        "new",
        sol::no_constructor,
        "name",
        &Project::Config::name,
        "version",
        &Project::Config::version,
        "description",
        &Project::Config::description,
        "authors",
        &Project::Config::authors,
        "dependencies",
        &Project::Config::dependencies,
        "toolchains",
        &Project::Config::toolchains,
        "flags",
        &Project::Config::flags,
        "modes",
        &Project::Config::modes,
        "libs",
        &Project::Config::libs,
        "license",
        &Project::Config::license,
        "git",
        &Project::Config::git,
        "cmake_version",
        &Project::Config::cmake_version,
        //"build_command",
        //&Project::Config::build_command,
        "build_dir",
        &Project::Config::build_dir,
        "src_dir",
        &Project::Config::src_dir,
        "include_dir",
        &Project::Config::include_dir,
        "lang_version",
        &Project::Config::lang_version,
        "lang",
        &Project::Config::lang,
        "project_type",
        &Project::Config::type,
        "keywords",
        &Project::Config::keywords,
        "prompts",
        &Project::Config::prompts);

    lua->new_usertype<Project::ProjectPrompt>(
        "ProjectPrompt",
        "value",
        &Project::ProjectPrompt::value,
        "default",
        &Project::ProjectPrompt::default_value,
        "getstr",
        &Project::ProjectPrompt::get<std::string>,
        "getint",
        &Project::ProjectPrompt::get<int>,
        "getbool",
        &Project::ProjectPrompt::get<bool>,
        "getfloat",
        &Project::ProjectPrompt::get<float>);
    lua->new_usertype<Project::Local>("Local");
    lua->set_function("get_build_command",&Project::Local::getBuildCommand);
    lua->set_function("get_test_command",&Project::Local::getTestCommand);
    lua->set_function("get_run_command",&Project::Local::getRunCommand);
    lua->set_function("get_requested_jobs",&Project::Local::getRequestedJobs);
    lua->set_function("get_max_jobs",&Project::Local::getMaxJobs);
    lua->set_function("get_override_change_hash",&Project::Local::getOverrideChangeHash);
    lua->set_function("set_build_command",&Project::Local::setBuildCommand);
    lua->set_function("set_test_command",&Project::Local::setTestCommand);
    lua->set_function("set_run_command",&Project::Local::setRunCommand);
    lua->set_function("get_build_mode",&Project::Local::getBuildMode);
    lua->set_function("set_build_mode",&Project::Local::setBuildMode);
    lua->set_function("get_current_mode",&Project::Local::setBuildMode);
    lua->set_function("get_project_path",&Project::Local::getProjectPath);
    // clang-format on
  }

} // namespace Frate::Lua
