#pragma once
#include "Frate/Dependency.hpp"
#include <Frate/Mode.hpp>
#include <Frate/Package.hpp>
#include <Frate/ProjectPrompt.hpp>
#include <Frate/RemoteServer.hpp>
#include <Frate/Template.hpp>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace Frate::Command {
  // class Template;
  // class RemoteServer;
  // class Mode;
  // class ProjectPrompt;
  using nlohmann::json;

  class Project {

  public:
    Project();
    bool loaded_json{false};
    std::string name;
    std::string description;
    std::string type{""};
    RemoteServer build_server;
    std::filesystem::path path;
    std::string git{"null"};
    std::string homepage{"null"};
    std::string bugs{"null"};
    std::string lang{"cpp"};
    std::string cmake_version{"3.20"};
    std::string lang_version{"20"};
    std::string compiler{"g++"};
    std::string license{""};
    std::string default_mode{"Release"};
    std::string build_command{"cmake --build ."};
    std::string test_command{"ctest"};
    std::string run_command{"./bin/"};
    std::vector<Mode> modes{Mode("Release", {"-O2"}), Mode("Debug", {"-g"}),
                            Mode("Test", {"-g"})};
    std::vector<std::string> authors{};
    std::vector<std::string> keywords{};
    std::string src_dir{"src"};
    std::string include_dir{"include"};
    std::vector<RemoteServer> build_servers{};
    std::vector<Dependency> dependencies{};
    std::string build_dir{"build"};
    // Package testing_lib;
    std::string version{"0.0.1"};
    std::vector<std::string> flags{};
    std::vector<std::string> toolchains{};
    std::vector<std::string> libs{};
    bool load();
    bool save();
    void checkKeys(json j);

    std::string getPath() { return path.string(); };

    std::unordered_map<std::string, ProjectPrompt> prompts{};
    std::unordered_map<std::string, json> global{};
    friend void from_json(const json &json_obj, Project &pro);
    friend void to_json(json &json_obj, const Project &pro);
    void fromTemplate(Template &t);
  };
} // namespace Frate::Command
