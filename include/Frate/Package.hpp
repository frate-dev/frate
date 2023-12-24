#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <sol/sol.hpp>

namespace Frate::Command{ 
  using nlohmann::json;
  class Package {
    public:
      std::string name;
      std::string git;
      std::string git_short;
      std::string git_prefixed;
      std::vector<std::string> versions;
      std::string target_link;
      std::string description;
      // std::string selected_version;
      std::string git_description;
      std::string language;
      std::string license;
      std::string owner;
      std::string owner_type;
      int stars;
      int forks;
      int open_issues;
      int watchers;
      // int score;
      bool addCallback(sol::state &lua);
      friend void from_json(const json &j, Package& package);
      friend void to_json(json &j, const Package& package);
      Package(){};

  };
}
