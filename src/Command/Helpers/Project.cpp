#include "Frate/Utils/General.hpp"
#include <Frate/Generators.hpp>
#include <fstream>
#include <nlohmann/json_fwd.hpp>
#include <Frate/Project.hpp>
#include <Frate/Template.hpp>
#include <Frate/ProjectPrompt.hpp>

namespace Frate::Command {
  Project::Project(){};
  bool Project::save(){
    std::ofstream file;
    std::string file_name = "frate-project.json";
    if(!std::filesystem::exists(this->path)){
      try{
        std::filesystem::create_directory(this->path);
      }catch(std::exception &e){
        Utils::debug(e.what());
        Utils::error << "Error while creating directory: " << this->path << std::endl;
        return false;
      }
    }

    if(!std::filesystem::exists(this->path / file_name)){
      std::cout << "Creating new project file" << std::endl;
    }

    json new_json = *this;

    Utils::info << "Writing to file: " << this->path / file_name << std::endl;

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
  void Project::fromTemplate(Template &t){
    this->src_dir = t.src_dir;
    this->include_dir = t.include_dir;
    this->build_dir = t.build_dir;
    this->dependencies = t.dependencies;
    this->default_mode = t.default_mode;
    this->modes = t.modes;
    this->flags = t.flags;
    // Check is toolchain is present
    // this->toolchains = t.toolchains;
    this->variables = t.variables;
    this->prompts = t.prompts;
    this->libs = t.system_libs;
  }
  void from_json (const json &j, Project& pro){
    pro.name = j.contains("name") ? j["name"] : "";
    pro.cmake_version = j.contains("cmake_version") ? j["cmake_version"] : "";
    pro.version = j.contains("version") ? j["version"] : "";
    pro.type = j.contains("type") ? j["type"] : "";
    pro.lang = j.contains("lang") ? j["lang"] : "";
    pro.lang_version = j.contains("lang_version") ? j["lang_version"] : "";
    pro.compiler = j.contains("compiler") ? j["compiler"] : "";
    pro.src_dir = j.contains("src_dir") ? j["src_dir"] : "";
    pro.build_dir = j.contains("build_dir") ? j["build_dir"] : "";
    pro.build_command = j.contains("build_command") ? j["build_command"] : "";
    pro.include_dir = j.contains("include_dir") ? j["include_dir"] : "";
    if(j.contains("authors")){
      pro.authors = j["authors"];
    }
    pro.type = j.contains("type") ? j["type"] : "";
    pro.description = j.contains("description") ? j["description"] : "";
    pro.default_mode = j.contains("default_mode") ? j["default_mode"] : "";
    if(j.contains("keywords")){
      pro.keywords = j["keywords"];
    }
    if(j.contains("libs")){
      pro.libs = j["libs"];
    }
    if(j.contains("dependencies")){
      pro.dependencies = j["dependencies"];
    }
    if(j.contains("flags")){
      pro.flags = j["flags"];
    }
    if(j.contains("toolchains")){
      pro.toolchains = j["toolchains"];
    }
    if(j.contains("prompts")){
      pro.prompts = j["prompts"];
    }
    if(j.contains("variables")){
      pro.variables = j["variables"];
    }
    if(j.contains("modes")){
      pro.modes = j["modes"];
    }
    pro.git = j.contains("git") ? j["git"] : "";
    pro.homepage = j.contains("homepage") ? j["homepage"] : "";
    pro.bugs = j.contains("bugs") ? j["bugs"] : "";
  }
  void to_json(json &j, const Project& pro){
    j["name"] = pro.name;
    j["cmake_version"] = pro.cmake_version;
    j["version"] = pro.version;
    j["type"] = pro.type;
    j["lang"] = pro.lang;
    j["lang_version"] = pro.lang_version;
    j["compiler"] = pro.compiler;
    j["src_dir"] = pro.src_dir;
    j["build_dir"] = pro.build_dir;
    j["include_dir"] = pro.include_dir;
    j["authors"] = pro.authors;
    j["type"] = pro.type;
    j["description"] = pro.description;
    j["default_mode"] = pro.default_mode;
    j["keywords"] = pro.keywords;
    j["libs"] = pro.libs;
    j["dependencies"] = pro.dependencies;
    j["flags"] = pro.flags;
    j["toolchains"] = pro.toolchains;
    j["prompts"] = pro.prompts;
    j["variables"] = pro.variables;
    j["modes"] = pro.modes;
    j["git"] = pro.git;
    j["homepage"] = pro.homepage;
    j["bugs"] = pro.bugs;
  }


  void Project::checkKeys(json j){
    std::vector<std::pair<std::string,bool>> required_keys = {
      {"name", false},
      {"type", false},
      {"cmake_version", false},
      {"version", false},
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
      {"type", false},
      {"description", false},
      {"toolchains", false},
      {"prompts", false}
    };
    for (std::pair<std::string, bool> &key: required_keys){
      if (!j.contains(key.first)){
        Utils::error << "Missing required key: " << key.first << std::endl;
      } else {
        key.second = true;
      }
    }
  }
  bool Interface::loadProjectJson() {
    using nlohmann::json;
    std::fstream file;
    std::string file_name = "frate-project.json";

    Utils::info << "Loading: " << (pro->path / file_name) << std::endl;

    if(project_present){
      return true;
    }

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
      std::filesystem::path current_path = pro->path;
      json j = json::parse(file);
      Project temp = j;
      pro = std::make_shared<Project>(temp);
      pro->path = current_path;
    }catch(std::exception &e){
      Utils::debug(e.what());
      return false;
    }

    project_present = true;

    return true;
  };
  bool Project::load(){
    using nlohmann::json;
    std::fstream file;
    std::string file_name = "frate-project.json";

    Utils::info << "Loading: " << (this->path / file_name) << std::endl;

    if(!std::filesystem::exists(this->path / file_name)){
      return false;
    }

    try{
      file.open(this->path / file_name);
      std::filesystem::path current_path = this->path;
      json j = json::parse(file);
      Project temp = j;
      *this = temp;
      this->path = current_path;
    }catch(std::exception &e){
      Utils::debug(e.what());
      return false;
    }

    return true;
  }
}
