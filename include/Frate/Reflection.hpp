#pragma once
#include <Frate/Command.hpp>
#include <nlohmann/json.hpp>

namespace Frate::Reflection {
  using nlohmann::json;
  using Command::Project;

  static void to_json(json &j, const Project &p) {
    j = json{
      {"name", p.name},
      {"description", p.description},
      {"type", p.type},
      {"version", p.version},
      {"authors", p.authors},
      {"license", p.license},
      {"dependencies", p.dependencies},
      {"keywords", p.keywords},
      {"git", p.git},
      {"homepage", p.homepage},
      {"bugs", p.bugs},
      {"compiler", p.compiler},
      {"lang", p.lang},
      {"lang_version", p.lang_version},
      {"build_dir", p.build_dir},
      {"include_dir", p.include_dir},
      {"src_dir", p.src_dir},
      {"toolchains", p.toolchains},
      {"flags", p.flags},
      {"libs", p.libs},
      {"default_mode", p.default_mode},
      {"modes", p.modes}
    };
  }
  static void from_json(Command::Package& package, const json &j){
    package.name = j.at("name").get<std::string>();
    package.selected_version = j.at("version").get<std::string>();
    package.description = j.at("description").get<std::string>();
    package.license = j.at("license").get<std::string>();
  }
  static void from_json(Command::Mode& mode, const json &j){
    mode.name = j.at("name").get<std::string>();
    mode.flags = j.at("flags").get<std::vector<std::string>>();
    mode.dependencies = json::array();
    for(Command::Package &dep : j.at("dependencies")){
      mode.dependencies.push_back(dep);
    }
  }
  static void from_json(Project& pro, const json &j){
    pro.name = j.at("name").get<std::string>();
    pro.description = j.at("description").get<std::string>();
    pro.type = j.at("type").get<std::string>();
    pro.version = j.at("version").get<std::string>();
    pro.authors = j.at("authors").get<std::vector<std::string>>();
    pro.license = j.at("license").get<std::string>();
    pro.dependencies = j.at("dependencies").get<std::vector<Command::Package>>();
    pro.keywords = j.at("keywords").get<std::vector<std::string>>();
    pro.git = j.at("git").get<std::string>();
    pro.homepage = j.at("homepage").get<std::string>();
    pro.bugs = j.at("bugs").get<std::string>();
    pro.compiler = j.at("compiler").get<std::string>();
    pro.lang = j.at("lang").get<std::string>();
    pro.lang_version = j.at("lang_version").get<std::string>();
    pro.build_dir = j.at("build_dir").get<std::string>();
    pro.include_dir = j.at("include_dir").get<std::string>();
    pro.src_dir = j.at("src_dir").get<std::string>();
    pro.toolchains = j.at("toolchains").get<std::vector<std::string>>();
    pro.flags = j.at("flags").get<std::vector<std::string>>();
    pro.libs = j.at("libs").get<std::vector<std::string>>();
    pro.default_mode = j.at("default_mode").get<std::string>();
    pro.modes = j.at("modes").get<std::vector<Command::Mode>>();
  }
}
