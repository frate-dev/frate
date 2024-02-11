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

} // namespace Frate::Lua
