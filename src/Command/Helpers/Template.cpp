#include <Frate/Template.hpp>
#include <Frate/Utils/Logging.hpp>
#include <Frate/Project.hpp>
#include "Frate/Dependency.hpp"

namespace Frate::Command {

  Template::Template(){

  }
  void from_json(const json &j, Template& t){
    Utils::info << "Template from json" << std::endl;
    t.name = j["name"];
    t.description = j["description"];
    t.version = j["version"];
    t.src_dir = j["src_dir"];
    t.include_dir = j["include_dir"];
    t.build_dir = j["build_dir"];
    t.dependencies = j["dependencies"].get<std::vector<Dependency>>();
    t.default_mode = j["default_mode"];
    t.modes = j["modes"].get<std::vector<Mode>>();
    t.flags = j["flags"].get<std::vector<std::string>>();
    t.system_libs = j["system_libs"].get<std::vector<std::string>>();
    t.supported_toolchains = j["supported_toolchains"].get<std::vector<std::string>>();
    t.supported_languages = j["supported_languages"].get<std::vector<std::string>>();
    t.supported_language_versions = j["supported_language_versions"].get<std::vector<std::string>>();
    t.min_cmake_version = j["min_cmake_version"];
    t.supported_compilers = j["supported_compilers"].get<std::vector<std::string>>();
    t.keywords = j["keywords"].get<std::vector<std::string>>();
    t.prompts = j["prompts"].get<std::unordered_map<std::string,ProjectPrompt>>();
    t.variables = j["variables"].get<std::unordered_map<std::string,json>>();
    Utils::info << "Template from json end" << std::endl;
  }

  void to_json(json &j, const Template& t){
    Utils::info << "Template to json" << std::endl;
    j["name"] = t.name;
    j["description"] = t.description;
    j["version"] = t.version;
    j["src_dir"] = t.src_dir;
    j["include_dir"] = t.include_dir;
    j["build_dir"] = t.build_dir;
    j["dependencies"] = t.dependencies;
    j["default_mode"] = t.default_mode;
    j["modes"] = t.modes;
    j["flags"] = t.flags;
    j["system_libs"] = t.system_libs;
    j["supported_toolchains"] = t.supported_toolchains;
    j["supported_languages"] = t.supported_languages;
    j["supported_languages_versions"] = t.supported_language_versions;
    j["min_cmake_version"] = t.min_cmake_version;
    j["supported_compilers"] = t.supported_compilers;
    j["keywords"] = t.keywords;
    j["prompts"] = t.prompts;
    j["variables"] = t.variables;
    Utils::info << "Template to json end" << std::endl;
  }

  void to_project(Project &pro, const Template &t){
    Utils::info << "Template to project" << std::endl;
    pro.name = t.name;
    pro.description = t.description;
    pro.version = t.version;
    pro.src_dir = t.src_dir;
    pro.include_dir = t.include_dir;
    pro.build_dir = t.build_dir;
    pro.dependencies = t.dependencies;
    pro.default_mode = t.default_mode;
    pro.modes = t.modes;
    Utils::info << "Template to project end" << std::endl;
  }
};
