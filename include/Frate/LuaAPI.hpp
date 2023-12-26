#pragma once
#include "Frate/Project.hpp"
#include "inja.hpp"
#include <filesystem>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <sol/forward.hpp>
#include <sol/sol.hpp>

namespace Frate::LuaAPI {
  using Command::Project;
  using nlohmann::json;
  using std::filesystem::path;

  class FrateApi {
  public:
    FrateApi() = default;
    ~FrateApi() = default;
    static std::string get_os();
    static std::vector<std::string> get_paths_recurse(std::string input_path);
    static std::string get_path();

    static std::string
    format(const std::string &str, sol::table in_table, sol::this_state s);

    static void print_table(sol::table in_table);
    static std::string fetch_text(const std::string &url);
    static sol::table fetch_json(const std::string &url, sol::this_state lua);
  };

  sol::table to_table(nlohmann::json json, sol::state_view &lua);
  nlohmann::json from_table(sol::table in_table);

  /*
   * Registers the project scripts with the project that is specifed
   */
  bool registerProjectScripts(inja::Environment &env,
                              sol::state &lua,
                              path script_path,
                              std::shared_ptr<Project> project);
  /*
   * Registers the project with the user types for the project struct
   */
  bool registerProject(sol::state &lua, std::shared_ptr<Command::Project> pro);
  /*
   * Registers api functions for the lua state
   * and initializes the lua state
   */
  void registerAPI(sol::state &lua);

  /*
   * Runs __init__ scripts
   */
  bool initScripts(sol::state &lua, std::shared_ptr<Project> project);

  /*
   * Runs __post__ scripts
   */
  bool postScripts(sol::state &lua, std::shared_ptr<Project> project);
} // namespace Frate::LuaAPI
