#include "Frate/Utils/General.hpp"
#include <Frate/Command.hpp>
#include <Frate/Generators.hpp>
#include <memory>


namespace Frate::Command {

  bool Project::save(){
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
      std::vector<std::pair<std::string,bool>> required_keys = {
          {"project_name", false},
          {"project_type", false},
          {"cmake_version", false},
          {"project_version", false},
          {"lang", false},
          {"lang_version", false},
          {"compiler", false},
          {"src_dir", false},
          {"build_dir", false},
          {"include_dir", false},
          {"default_mode", false},
          {"modes", false},
          {"dependencies", false},
          {"flags", false},
          {"keywords", false},
          {"authors", false},
          {"project_path", false},
          {"project_type", false},
          {"project_description", false},
          {"toolchains", false}
      };
      for (std::pair<std::string, bool> &key: required_keys){
        if (j.find(key.first) == j.end()){
          Frate::error << "Missing required key: " << key.first << std::endl;
        } else {
          key.second = true;
        }
      }
    }
}
