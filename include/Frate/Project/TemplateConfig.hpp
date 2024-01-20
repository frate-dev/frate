#pragma once
#include "Frate/Dependency.hpp"

#include "Frate/Project/Mode.hpp"
#include "Frate/Project/ProjectPrompt.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace Frate::Project {
  using nlohmann::json;
  class Config;

  class TemplateConfig {
  public:
    TemplateConfig();
    std::string name;
    std::string description;
    std::string version;
    std::string src_dir{"src"};
    std::string include_dir{"include"};
    std::string build_dir{"build"};
    std::vector<Command::Dependency> dependencies{};
    std::string default_mode{"Release"};
    std::vector<Mode> modes{};
    std::vector<std::string> flags{};
    std::vector<std::string> system_libs{};
    std::vector<std::string> supported_toolchains{};
    std::vector<std::string> supported_languages{};
    std::vector<std::string> supported_language_versions{};
    std::string min_cmake_version{"3.10"};
    std::vector<std::string> supported_compilers{};
    std::vector<std::string> keywords{};
    std::unordered_map<std::string, ProjectPrompt> prompts{};
    std::unordered_map<std::string, json> global{};
    friend void from_json(const json &json_obj, TemplateConfig &temp);
    friend void to_json(json &json_obj, const TemplateConfig &temp);
    friend void to_project(Config &pro, const TemplateConfig &temp);
  };
} // namespace Frate::Project
