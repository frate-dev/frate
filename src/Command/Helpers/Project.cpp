#include "Frate/Utils/General.hpp"
#include <Frate/Command.hpp>
#include <Frate/Generators.hpp>
#include <memory>
#include <fstream>


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
    //Generators::CMakeList::createCMakeLists(std::make_shared<Project>(*this));
    return true;
  }
  /*
   * Welp reflection is a bitch aint it
   */
  void Project::fromJson(json j){
    checkKeys(j);
    try{
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
        d.name = dep["name"].is_null() ? "" : dep["name"].is_null() ? "" : dep["name"];
        d.git = dep["git"].is_null() ? "" : dep["git"].is_null() ? "" : dep["git"];
        d.selected_version = dep["version"].is_null() ? "" : dep["version"];
        d.target_link = dep["target_link"].is_null() ? "" : dep["target_link"];
        if(d.target_link.empty()){
          warning <<
            "Warning: target_link is empty for dependency: " << d.name << std::endl;
        }
        dependencies.push_back(d);
      }
      std::vector<Mode> temp_modes;
      for (auto &mode: j["modes"]){
        Mode m;
        m.name = mode["name"].is_null() ? "" : mode["name"];
        m.flags = mode["flags"];
        for (auto &dep : mode["dependencies"]) {
          Package new_dep;
          new_dep.name = dep["name"].is_null() ? "" : dep["name"];
          new_dep.git = dep["git"].is_null() ? "" : dep["git"];
          new_dep.selected_version = dep["version"].is_null() ? "" : dep["version"];
          new_dep.target_link = dep["target_link"].is_null() ? "" : dep["target_link"];
          if(new_dep.target_link == ""){
            warning <<
              "Warning: target_link is empty for dependency: " << new_dep.name <<
              " in mode: " << m.name << std::endl;
          }
          
          m.dependencies.push_back(new_dep);
        }
        temp_modes.push_back(m);
      }
      modes = temp_modes;
      flags = j["flags"];
      toolchains = j["toolchains"];
      if(!j.contains("prompts")){
        prompts = {};
      }else{
        for(auto [key, value] : j["prompts"].items()){
          ProjectPrompt prompt;

          prompt.type = value["type"].is_null() ? "string" : value["type"];

          prompt.text = value["text"].is_null() ? "missing prompt text" : value["text"];


          if(value["options"].is_null()){
            prompt.options = {};
          }else{
            for(auto &option : value["options"]){
              prompt.options.push_back(option);
            }
          }


          if(value["default_value"].is_null()){
            prompt.default_value = "";
          }else{
            prompt.default_value = value["default_value"];
          }

          prompts[key] = prompt;
        }
      }
    } catch (std::exception &e){
      Utils::debug(e.what());
      error << "Error while parsing json for project" << std::endl;
    }
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
      j["project_type"] = project_type;
      j["project_description"] = project_description;
      j["toolchains"] = toolchains;
      j["prompts"] = json::object();
      for(auto [key, value] : prompts){
        json prompt;
        prompt["type"] = value.type;
        prompt["text"] = value.text;
        prompt["options"] = value.options;
        prompt["default_value"] = value.default_value;
        j["prompts"][key] = prompt;
      }
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
          {"project_type", false},
          {"project_description", false},
          {"toolchains", false},
          {"prompts", false}
      };
      for (std::pair<std::string, bool> &key: required_keys){
        if (!j.contains(key.first)){
          Frate::error << "Missing required key: " << key.first << std::endl;
        } else {
          key.second = true;
        }
      }
    }
}
