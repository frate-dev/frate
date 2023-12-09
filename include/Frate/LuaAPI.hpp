#pragma once
#include "Frate/Command.hpp"
#include "inja.hpp"
#include <nlohmann/json.hpp>
#include <sol/sol.hpp>
#include <filesystem>




namespace Frate::LuaAPI {
  using std::filesystem::path;


  sol::table to_table(const nlohmann::json& json);
  nlohmann::json from_table(sol::table in_table);

  std::string format(const std::string &str, sol::variadic_args var_args);
  /*
   * Registers the project scripts with the project that is specifed
   */
  bool registerProjectScripts(inja::Environment &env, sol::state &lua, path script_path);
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
