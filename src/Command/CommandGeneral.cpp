#include "Command.hpp"
#include <toml++/toml.hpp>
#include <cstddef>
#include <format>
#include <iostream>

namespace Command {
  bool Interface::LoadPackageToml() {
    try {
      std::string file_name = "config.toml";



      auto data = toml::parse_file((ctx->project_path / file_name).string());
      ctx->project_name = data["project"]["project_name"].value_or("");
      for (auto &author : *data["project"]["authors"].as_array()) {
        ctx->authors.push_back(author.value_or(""));
      }
      ctx->src_dir = data["project"]["src_dir"].value_or("");
      ctx->build_dir = data["project"]["build_dir"].value_or("");
      ctx->compiler = data["project"]["compiler"].value_or("");
      ctx->cmake_version = data["project"]["cmake_version"].value_or("");
      ctx->git = data["project"]["git"].value_or("");
      ctx->lang = data["project"]["lang"].value_or("");
      ctx->include_dir =
        data["project"]["include_dir"].value_or("");
      ctx->lang_version =
        data["project"]["lang_version"].value_or("");
      ctx->project_version = data["project"]["project_version"].value_or("");
      if (data.at_path("dependencies").is_table()) {
        for (auto &dep : *data["dependencies"].as_table()) {

          Command::dependency _dep;
          _dep.name = dep.first;
          _dep.url =
            data["dependencies"][dep.first].as_array()->at(0).value_or("");
          _dep.version =
            data["dependencies"][dep.first].as_array()->at(1).value_or("");
          _dep.target_link = data["dependencies"][dep.first].as_array()->at(2).value_or("");

          ctx->dependencies.push_back(_dep);

        }
      }

    } catch (const toml::parse_error &err) {
      std::cout << "Error: Could not load config.toml" << std::endl;
      return false;
    }

    return true;
  };

} // namespace Command
