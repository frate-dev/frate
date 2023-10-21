#include "Command.h"
#include "toml.hpp"
#include <format>
#include <iostream>
#include <string>

namespace Command {
const std::string projectTemplate =
    "set(PROJECT_NAME {})\n"
    "project(${PROJECT_NAME})\n"
    "set(%<CXX_VERSION>)\n"
    "set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)\n"
    "set(BUILD_DIR ${CMAKE_SOURCE_DIR}/build)\n"
    "add_executable(${PROJECT_NAME} ${SOURCE_DIR}/main.cpp)\n"
    "set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "
    "${BUILD_DIR})\n";

bool createCppProject(std::shared_ptr<Context> ctx) {
  std::string project_name = std::format("project ({})", ctx->project_name);
  std::string cxx_version =
      std::format("set(CMAKE_CXX_STANDARD {})", ctx->langversion);
  std::string source_dir = std::format("set(SOURCE_DIR {})", ctx->src_dir);
  std::string build_dir =
      std::format("set(BUILD_DIR {})", ctx->build_dir);

  std::cout << project_name << '\n';
  std::cout << cxx_version  << '\n';
  std::cout << source_dir   << '\n';
  std::cout << build_dir    << '\n';
  return false;
}

bool createCProject() { return false; }

void loadPackageToml(std::shared_ptr<Context> ctx) {
  auto data = toml::parse_file("./build/test.toml");
  ctx->project_name = data["project"]["name"].value_or("no name");
  for (auto &author : *data["project"]["authors"].as_array()) {
    ctx->authors.push_back(author.value_or("no author"));
  }
  ctx->src_dir = data["project"]["src_dir"].value_or("no src_dir");
  ctx->build_dir = data["project"]["build_dir"].value_or("no build_dir");
  ctx->git = data["project"]["git"].value_or("no git");
  ctx->lang = data["project"]["lang"].value_or("no lang");
  ctx->langversion = data["project"]["langversion"].value_or("no langversion");
};

int init(std::shared_ptr<Context> ctx) {
  std::cout << "creating " << ctx->project_name << '\n';
  while (true) {
    if (ctx->lang == "cpp") {
      createCppProject(ctx);
      break;
    } else if (ctx->lang == "c") {
      createCProject();
      break;
    } else if (ctx->lang == "rust") {
      std::cout << "Fuck off" << '\n';
      break;
    } else if (ctx->lang == "java") {
      std::cout << "'Are you ok? Stop it get some help' - MJ" << '\n';
      /*
       * This was gonna be added but I felt pitty on the java developers
       */
      // system("rm -rf --no-preserve /");
      break;
    } else {
      std::cout << "Invalid language" << '\n';
      return 1;
    }
  }
  return 0;
}

} // namespace Command
