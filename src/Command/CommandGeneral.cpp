#include "Command.hpp"
#include "toml.hpp"
#include <cstddef>
#include <format>
#include <iostream>

namespace Command {
  bool loadPackageToml(std::shared_ptr<Context> ctx) {
    try {
      std::string file_name = "config.toml";
#ifdef DEBUG
      file_name = "build/config.toml";
      std::cout << "Loading config.toml from " << ctx->project_path / file_name
        << std::endl;
#endif

      auto data = toml::parse_file((ctx->project_path / file_name).string());
      ctx->project_name = data["project"]["project_name"].value_or("no name");
      for (auto &author : *data["project"]["authors"].as_array()) {
        ctx->authors.push_back(author.value_or("no author"));
      }
      ctx->src_dir = data["project"]["src_dir"].value_or("no src_dir");
      ctx->build_dir = data["project"]["build_dir"].value_or("no build_dir");
      ctx->compiler = data["project"]["compiler"].value_or("no compiler");
      ctx->cmake_version = data["project"]["cmake_version"].value_or("");
      ctx->git = data["project"]["git"].value_or("no git");
      ctx->lang = data["project"]["lang"].value_or("no lang");
      ctx->include_dir =
        data["project"]["include_dir"].value_or("no include_dir");
      ctx->lang_version =
        data["project"]["lang_version"].value_or("no langversion");
      ctx->project_version = data["project"]["project_version"].value_or("0.0.1");
      if (data.at_path("dependencies").is_table()) {
        for (auto &dep : *data["dependencies"].as_table()) {

          Command::dependency _dep;
          _dep.name = dep.first;
          _dep.url =
            data["dependencies"][dep.first].as_array()->at(0).value_or("");
          _dep.version =
            data["dependencies"][dep.first].as_array()->at(1).value_or("");
          ctx->dependencies.push_back(_dep);
        }
      }

    } catch (const toml::parse_error &err) {
      std::cout << "Error: Could not load config.toml" << std::endl;
      return false;
    }

    return true;
  };
  bool writePackageToml(std::shared_ptr<Context> ctx) {
    toml::array authors = toml::array{};
    toml::array flags = toml::array{};
    for (auto &flag : ctx->flags) {
      flags.push_back(flag);
    }
    toml::table table = toml::table{
      {"project",
        toml::table{
          {"cmake_version", ctx->cmake_version},
          {"include_dir", ctx->include_dir},
          {"project_version", ctx->project_version},
          {"compiler", ctx->compiler},
          {"project_name", ctx->project_name},
          {"authors", authors},
          {"src_dir", ctx->src_dir},
          {"build_dir", ctx->build_dir},
          {"lang", ctx->lang},
          {"lang_version", ctx->lang_version},
          {"cflags", flags},
        }},
    };

    toml::table deps_table = toml::table{{"dependencies", toml::table{}}};

    for (Command::dependency &dep : ctx->dependencies) {

      toml::array deps_values = toml::array{};
      deps_values.push_back(dep.url);
      deps_values.push_back(dep.version);
      deps_table["dependencies"].as_table()->insert(dep.name, deps_values);
    }

    std::ofstream file;
    std::string file_name = "config.toml";
#ifdef DEBUG
    file_name = "build/config.toml";
    std::cout << "Writing config.toml to " << ctx->project_path / file_name
      << std::endl;
#endif
    file.open(ctx->project_path / file_name);
    file << table;
    file << '\n';
    file << deps_table;
    file << '\n';
    file.close();
    return true;
  }
} // namespace Command
