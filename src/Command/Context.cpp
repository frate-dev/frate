#include "Command.hpp"


namespace Command {
  /*
   * Welp reflection is a bitch aint it
   */
    void Project::fromJson(json j){
    #ifdef DEBUG
      project_path = std::filesystem::current_path() / "build";
    #else
      project_path = std::filesystem::current_path();
    #endif
      project_name = j["project_name"];
      cmake_version = j["cmake_version"];
      project_version = j["project_version"];
      lang = j["lang"];
      lang_version = j["lang_version"];
      compiler = j["compiler"];
      src_dir = j["src_dir"];
      build_dir = j["build_dir"];
      include_dir = j["include_dir"];
      authors = j["authors"];
      project_type = j["project_type"];
      project_description = j["project_description"];
      for (auto &dep : j["dependencies"]) {
        Dependency d;
        d.name = dep["name"];
        d.url = dep["url"];
        d.version = dep["version"];
        d.target_link = dep["target_link"];
        dependencies.push_back(d);
      }
      flags = j["flags"];
    }
    nlohmann::json Project::toJson(){
      using nlohmann::json;
      std::vector<json> deps;
      for (auto &dep : dependencies) {
        json dep_json;
        dep_json["name"] = dep.name;
        dep_json["url"] = dep.url;
        dep_json["version"] = dep.version;
        dep_json["target_link"] = dep.target_link;
        deps.push_back(dep_json);
      }
      json j;
      j["project_name"] = project_name;
      j["cmake_version"] = cmake_version;
      j["project_version"] = project_version;
      j["lang"] = lang;
      j["lang_version"] = lang_version;
      j["compiler"] = compiler;
      j["src_dir"] = src_dir;
      j["build_dir"] = build_dir;
      j["include_dir"] = include_dir;
      j["dependencies"] = deps;
      j["flags"] = flags;
      j["authors"] = authors;
      j["project_path"] = project_path;
      j["project_type"] = project_type;
      j["project_description"] = project_description;
      return  j;

    };
}
