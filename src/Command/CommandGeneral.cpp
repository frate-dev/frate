#include "Command.hpp"
#include <toml++/toml.hpp>
#include <cstddef>
#include <format>
#include <iostream>

namespace Command {
  bool Interface::LoadPackageJson() {
    using nlohmann::json;
    try {
      std::string file_name = "config.json";
      std::fstream file;
      file.open((ctx->project_path / file_name).string());
      json data = json::parse(file);
      ctx->project_name = data["project_name"].template get<std::string>();
      ctx->authors = data["authors"].template get<std::vector<std::string>>();
      ctx->src_dir = data["src_dir"].template get<std::string>();
      ctx->build_dir = data["build_dir"].template get<std::string>();
      ctx->compiler = data["compiler"].template get<std::string>();
      ctx->cmake_version = data["cmake_version"].template get<std::string>();
      ctx->git = data["git"].template get<std::string>();
      ctx->lang = data["lang"].template get<std::string>();
      ctx->include_dir = data["include_dir"].template get<std::string>();
      ctx->lang_version = data["lang_version"].template get<std::string>();
      ctx->project_version = data["project_version"].template get<std::string>();
      for (json &dep: data["dependencies"]) {
        Command::dependency _dep;
        _dep.name = dep["name"].template get<std::string>();
        _dep.url = dep["url"].template get<std::string>();
        _dep.version = dep["version"].template get<std::string>();
        _dep.target_link = dep["target_link"].template get<std::string>();
        ctx->dependencies.push_back(_dep);
      }
      


      

    } catch (json::exception &e) {
      std::cout << "Error: Could not load config.json" << std::endl;
      return false;
    }

    return true;
  };

} // namespace Command
