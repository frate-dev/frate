#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace Frate::Project {
  using nlohmann::json;

  class ProjectPrompt {
  public:
    ProjectPrompt() = default;
    std::string value{""};
    std::string text{""};
    std::string type{"string"};
    std::string default_value{""};
    bool required{false};
    std::vector<std::string> options{};
    friend void from_json(const json &json_obj, ProjectPrompt &prompt);
    friend void to_json(json &json_obj, const ProjectPrompt &prompt);
    std::function<bool(std::string)> validator{[this](std::string s) -> bool {
      if (options.size() == 0) {
        return true;
      }
      else {
        for (std::string option : options) {
          if (s == option) {
            return true;
          }
        }
        return false;
      }
    }};
    template <typename T> T get();
  };
} // namespace Frate::Project
