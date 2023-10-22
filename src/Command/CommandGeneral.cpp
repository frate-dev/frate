#include "Command.h"
#include "toml.hpp"

namespace Command {
int loadPackageToml(std::shared_ptr<Context> ctx) {
  auto data = toml::parse_file("./config.toml");
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
  ctx->include_dir = data["project"]["include_dir"].value_or("no include_dir");
  ctx->lang_version =
      data["project"]["lang_version"].value_or("no langversion");
  ctx->project_version = data["project"]["project_version"].value_or("0.0.1");
  /* for (auto &flag : *data["project"]["cflags"].as_array()) { */
    /* ctx->flags.push_back(flag.value_or("no flag")); */
  /* } */
  return 0;
};
int writePackageToml(std::shared_ptr<Context> ctx) {
  toml::array authors = toml::array{};
  toml::array flags = toml::array{};
  for (auto &flag : ctx->flags) {
    flags.push_back(flag);
  }
  toml::table table = toml::table{
      {"project", toml::table{{"cmake_version", ctx->cmake_version},
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
  std::ofstream file;
  file.open("./config.toml");
  file << table;
  file << '\n';
  file.close();
  return 0;
}
} // namespace Command
