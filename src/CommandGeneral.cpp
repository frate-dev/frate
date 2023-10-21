#include "Command.h"
#include "toml.hpp"

namespace Command {
void loadPackageToml(std::shared_ptr<Context> ctx) {
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
  ctx->lang_version = data["project"]["lang_version"].value_or("no langversion");
  ctx->project_version = data["project"]["project_version"].value_or("0.0.1");
};
}
