#pragma once
#include "Frate/Command.hpp"
#include "inja.hpp"
#include <nlohmann/json.hpp>
#include <sol/sol.hpp>
#include <filesystem>




namespace Frate::LuaAPI {
  using std::filesystem::path;
  using nlohmann::json;
  using Command::Project;

  class FrateApi {
    public:
      FrateApi() = default;
      ~FrateApi() = default;
      static std::string get_os();
      static std::vector<std::string> get_paths_recurse(std::string input_path);
      static std::string get_path();
      static std::string format(const std::string &str, sol::variadic_args var_args);
  };

  sol::table to_table(nlohmann::json json, sol::state &lua);
  nlohmann::json from_table(sol::table in_table);

  /*
   * Registers the project scripts with the project that is specifed
   */
  bool registerProjectScripts(inja::Environment &env, sol::state &lua,
      path script_path, std::shared_ptr<Project> project);
  /*
   * Registers the project with the user types for the project struct
   */
  bool registerProject(sol::state &lua, std::shared_ptr<Command::Project> pro);
  /*
   * Registers api functions for the lua state 
   * and initializes the lua state
   */
  void registerAPI(sol::state &lua);
}
