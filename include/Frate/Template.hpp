#pragma once
#include "Frate/Dependency.hpp"
#include <Frate/Mode.hpp>
#include <Frate/ProjectPrompt.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace Frate::Command {
  // class Mode;
  // class Package;
  // class ProjectPrompt;
  class Project;
  using nlohmann::json;

  class Template {
  public:
    Template();
    std::string name;
    std::string description;
    std::string version;
    std::string src_dir{"src"};
    std::string include_dir{"include"};
    std::string build_dir{"build"};
    std::vector<Dependency> dependencies{};
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
    friend void from_json(const json &json_obj, Template &temp);
    friend void to_json(json &json_obj, const Template &temp);
    friend void to_project(Project &pro, const Template &temp);
  };

} // namespace Frate::Command
