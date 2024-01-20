#pragma once
#include "Frate/Dependency.hpp"
#include "Frate/Project/Mode.hpp"
#include "Frate/Project/ProjectPrompt.hpp"
#include "Frate/Project/TemplateConfig.hpp"
#include "Frate/Project/TemplateMeta.hpp"
#include <Frate/Package.hpp>
#include <Frate/RemoteServer.hpp>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace Frate::Project {

  class Config {

  public:
    Config() = default;
    bool loaded_json{false};
    std::filesystem::path template_path;
    TemplateMeta current_template;
    std::vector<std::filesystem::path> template_files;
    std::vector<std::filesystem::path> script_files;
    std::string name;
    std::string description;
    std::string type{""};
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
    std::vector<Command::RemoteServer> build_servers{};
    std::vector<Command::Dependency> dependencies{};
    std::string build_dir{"build"};
    // Package testing_lib;
    std::string version{"0.0.1"};
    std::vector<std::string> flags{};
    std::vector<std::string> toolchains{};
    std::vector<std::string> libs{};
    /*
     * This loads the project from the frate-project.json located at the root of
     * the project path
     */
    bool load();
    /*
     * This saves the project to the frate-project.json located at the root of
     * the project path
     */
    bool save();
    void checkKeys(json j);
    /*
     * is what you use if you are using the path in a command
     * This will add quotes around the path to avoid issues with spaces
     * for example: path/to/project some crazy path name
     * becomes: 'path/to/project some crazy path name'
     */
    std::string safePath();

    std::unordered_map<std::string, ProjectPrompt> prompts{};
    std::unordered_map<std::string, json> global{};
    friend void from_json(const json &json_obj, Config &pro);
    friend void to_json(json &json_obj, const Config &pro);
    void fromTemplate(TemplateConfig &_template);
  };

} // namespace Frate::Project
