#pragma once
#include "Frate/Project/Config.hpp"
#include <inja/inja.hpp>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <sol/forward.hpp>
#include <sol/sol.hpp>

namespace Frate::Lua {

  using nlohmann::json;
  using Project::Config;
  using std::filesystem::path;

  class FrateApi {
  public:
    FrateApi() = default;
    ~FrateApi() = default;
    static std::string get_os();
    static std::vector<std::string> get_paths_recurse(std::string input_path);
    static std::string get_path();

    static std::string format(const std::string &str, sol::table in_table,
                              sol::this_state s);

    static void print_table(sol::table in_table);
    static std::string fetch_text(const std::string &url);
    static sol::table fetch_json(const std::string &url, sol::this_state lua);
  };

  /*
   * Registers the project scripts with the project that is specifed
   */
  [[deprecated("Use TemplateEnvironment")]]
  bool registerProjectScripts(inja::Environment &env, sol::state &lua,
                              path script_path,
                              std::shared_ptr<Project::Config> project);

  /*
   * Registers the project with the user types for the project struct
   */
  [[deprecated("Use TemplateEnvironment")]]
  bool registerProject(sol::state &lua, std::shared_ptr<Project::Config> pro);
  /*
   * Registers api functions for the lua state
   * and initializes the lua state
   */
  [[deprecated("Use TemplateEnvironment")]]
  void registerAPI(sol::state &lua);

  /*
   * Runs __init__ scripts
   */
  [[deprecated("Use TemplateEnvironment")]]
  bool initScripts(sol::state &lua, std::shared_ptr<Project::Config> project);

  /*
   * Runs __post__ scripts
   */
  [[deprecated("Use TemplateEnvironment")]]
  bool postScripts(sol::state &lua, std::shared_ptr<Project::Config> project);
} // namespace Frate::Lua
