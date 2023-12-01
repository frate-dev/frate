#include <Frate/Command.hpp>
#include <Frate/Generators.hpp>
#include <memory>


namespace Command {

  bool Project::writeConfig(){
    std::ofstream file;
    std::string file_name = "frate-project.json";
    try {
      file.open(this->project_path / file_name);
    } catch (std::exception &e) {
      std::cout << "Error opening file: " << e.what() << std::endl;
      return false;
    }
    file << this->toJson().dump(2);
    file << '\n';
    file.close();
    Generators::CMakeList::createCMakeLists(std::make_shared<Project>(*this));
    return true;
  }
  /*
   * Welp reflection is a bitch aint it
   */
    void Project::fromJson(json j){
      project_name = j["project_name"];
      cmake_version = j["cmake_version"];
      project_version = j["project_version"];
      project_type = j["project_type"];
      lang = j["lang"];
      lang_version = j["lang_version"];
      compiler = j["compiler"];
      src_dir = j["src_dir"];
      build_dir = j["build_dir"];
      include_dir = j["include_dir"];
      authors = j["authors"];
      project_type = j["project_type"];
      project_description = j["project_description"];
      default_mode = j["default_mode"];
      keywords = j["keywords"];
      for (auto &dep : j["dependencies"]) {
        Package d;
        d.name = dep["name"];
        d.git = dep["git"];
        d.selected_version = dep["version"];
        d.target_link = dep["target_link"];
        dependencies.push_back(d);
      }
      std::vector<Mode> temp_modes;
      for (auto &mode: j["modes"]){
        Mode m;
        m.name = mode["name"];
        m.flags = mode["flags"];
        for (auto &dep : mode["dependencies"]) {
          Package d;
          d.name = dep["name"];
          d.git = dep["git"];
          d.selected_version = dep["version"];
          d.target_link = dep["target_link"];
          m.dependencies.push_back(d);
        }
        temp_modes.push_back(m);
      }
      modes = temp_modes;
      flags = j["flags"];
    }
    nlohmann::json Project::toJson(){
      using nlohmann::json;
      std::vector<json> deps;
      for (auto &dep : dependencies) {
        json dep_json;
        dep_json["name"] = dep.name;
        dep_json["git"] = dep.git;
        dep_json["version"] = dep.selected_version;
        dep_json["target_link"] = dep.target_link;
        deps.push_back(dep_json);
      }
      std::vector<json> modes_json;
      for (auto &mode : modes) {
        json mode_json;
        json mode_deps = json::array();
        for (Package dep: mode.dependencies) {
          json dep_json;
          dep_json["name"] = dep.name;
          dep_json["git"] = dep.git;
          dep_json["version"] = dep.selected_version;
          dep_json["target_link"] = dep.target_link;
          mode_deps.push_back(dep_json);
        }

        mode_json["name"] = mode.name;
        mode_json["flags"] = mode.flags;
        mode_json["dependencies"] = mode_deps;
        modes_json.push_back(mode_json);
      }
      json j;
      j["project_name"] = project_name;
      j["project_type"] = project_type;
      j["cmake_version"] = cmake_version;
      j["project_version"] = project_version;
      j["lang"] = lang;
      j["lang_version"] = lang_version;
      j["compiler"] = compiler;
      j["src_dir"] = src_dir;
      j["build_dir"] = build_dir;
      j["include_dir"] = include_dir;
      j["default_mode"] = default_mode;
      j["modes"] = modes_json;
      j["dependencies"] = deps;
      j["flags"] = flags;
      j["keywords"] = keywords;
      j["authors"] = authors;
      j["project_path"] = project_path;
      j["project_type"] = project_type;
      j["project_description"] = project_description;
      j["toolchains"] = toolchains;
      return  j;

    };
    void Project::checkKeys(json j){
      j.contains("project_name") ? j["project_name"] : throw std::runtime_error("project_name is required");
      j.contains("project_type") ? j["project_type"] : throw std::runtime_error("project_type is required");
      j.contains("cmake_version") ? j["cmake_version"] : throw std::runtime_error("cmake_version is required");
      j.contains("project_version") ? j["project_version"] : throw std::runtime_error("project_version is required");
      j.contains("lang") ? j["lang"] : throw std::runtime_error("lang is required");
      j.contains("lang_version") ? j["lang_version"] : throw std::runtime_error("lang_version is required");
      j.contains("compiler") ? j["compiler"] : throw std::runtime_error("compiler is required");
      j.contains("src_dir") ? j["src_dir"] : throw std::runtime_error("src_dir is required");
      j.contains("build_dir") ? j["build_dir"] : throw std::runtime_error("build_dir is required");
      j.contains("include_dir") ? j["include_dir"] : throw std::runtime_error("include_dir is required");
      j.contains("default_mode") ? j["default_mode"] : throw std::runtime_error("default_mode is required");
      j.contains("modes") ? j["modes"] : throw std::runtime_error("modes is required");
      j.contains("dependencies") ? j["dependencies"] : throw std::runtime_error("dependencies is required");
      j.contains("flags") ? j["flags"] : throw std::runtime_error("flags is required");
      j.contains("keywords") ? j["keywords"] : throw std::runtime_error("keywords is required");
      j.contains("authors") ? j["authors"] : throw std::runtime_error("authors is required");
      j.contains("project_path") ? j["project_path"] : throw std::runtime_error("project_path is required");
      j.contains("project_type") ? j["project_type"] : throw std::runtime_error("project_type is required");
      j.contains("project_description") ? j["project_description"] : throw std::runtime_error("project_description is required");
      j.contains("toolchains") ? j["toolchains"] : throw std::runtime_error("toolchains is required");
    }
}
