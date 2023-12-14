#include "Frate/Utils/General.hpp"
#include <Frate/Command.hpp>
#include <Frate/Generators.hpp>
#include <fstream>


namespace Frate::Command {

  bool Project::save(){
    std::ofstream file;
    std::string file_name = "frate-project.json";
    if(!std::filesystem::exists(this->path)){
      std::filesystem::create_directory(this->path);
    }

    if(!std::filesystem::exists(this->path / file_name)){
      std::cout << "Creating new project file" << std::endl;
    }

    json new_json = this->toJson();

    info << "Writing to file: " << this->path / file_name << std::endl;

    try{
      file.open(this->path / file_name);
    }catch(std::exception &e){
      Utils::debug(e.what());
      return false;
    }
    file << new_json.dump(2);

    //Generators::CMakeList::createCMakeLists(std::make_shared<Project>(*this));
    return true;
  }
  /*
   * Welp reflection is a bitch aint it
   */
  void Project::fromJson(json j){
    checkKeys(j);
    name = j["project_name"];
    cmake_version = j["cmake_version"];
    version = j["project_version"];
    type = j["project_type"];
    lang = j["lang"];
    lang_version = j["lang_version"];
    compiler = j["compiler"];
    src_dir = j["src_dir"];
    build_dir = j["build_dir"];
    include_dir = j["include_dir"];
    authors = j["authors"];
    type = j["project_type"];
    description = j["project_description"];
    default_mode = j["default_mode"];
    keywords = j["keywords"];
    libs = j["libs"];
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
    if(j.contains("variables")){
      for(auto [key, value] : j["variables"].items()){
        variables[key] = value;
      }
    }else{
      variables = json::object();
    }

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

  }
    nlohmann::json Project::toJson(){
      using nlohmann::json;
      json new_json;
      new_json["dependencies"] = json::array();
      for (auto &dep : dependencies) {
        json dep_json;
        dep_json["name"] = dep.name;
        dep_json["git"] = dep.git;
        dep_json["version"] = dep.selected_version;
        dep_json["target_link"] = dep.target_link;
        new_json["dependencies"].push_back(dep_json);
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
      new_json["project_name"] = name;
      new_json["project_type"] = type;
      new_json["cmake_version"] = cmake_version;
      new_json["project_version"] = version;
      new_json["lang"] = lang;
      new_json["lang_version"] = lang_version;
      new_json["compiler"] = compiler;
      new_json["src_dir"] = src_dir;
      new_json["build_dir"] = build_dir;
      new_json["include_dir"] = include_dir;
      new_json["default_mode"] = default_mode;
      new_json["modes"] = modes_json;
      new_json["flags"] = flags;
      new_json["keywords"] = keywords;
      new_json["authors"] = authors;
      new_json["project_type"] = type;
      new_json["project_description"] = description;
      new_json["toolchains"] = toolchains;
      new_json["prompts"] = json::object();
      new_json["libs"] = libs;
      for(auto [key, value] : prompts){
        json prompt;
        prompt["value"] = value.value;
        prompt["type"] = value.type;
        prompt["text"] = value.text;
        prompt["options"] = value.options;
        prompt["default_value"] = value.default_value;
        new_json["prompts"][key] = prompt;
      }
      for(auto [key, value] : variables){
        new_json["variables"][key] = value;
      }


      return  new_json;

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
  bool Interface::LoadProjectJson() {
    using nlohmann::json;
    std::fstream file;
    std::string file_name = "frate-project.json";

    Frate::info << "Loading: " << (pro->path / file_name) << std::endl;

    if(!std::filesystem::exists(pro->path / file_name)){
      return false;
    }


    try{
      file.open(pro->path / file_name);
    }catch(std::exception &e){
      Utils::debug(e.what());
      return false;
    }

    try{
      json j = json::parse(file);
      pro->fromJson(j);
    }catch(std::exception &e){
      Utils::debug(e.what());
      return false;
    }

    return true;
  };
}
