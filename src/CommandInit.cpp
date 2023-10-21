#include "Command.h"
#include "toml.hpp"
#include <format>
#include <fstream>
#include <iostream>
#include <string>

namespace Command {

bool createCMakelists(std::shared_ptr<Context> ctx) {
  std::string cmake_minimum_required =
      std::format("cmake_minimum_required(VERSION {})", ctx->cmake_version);
  std::string project_name = std::format("project ( \n\
    {} \n\
    VERSION {}\n\
    LANGUAGES CXX\n\
)",
                                         ctx->project_name, ctx->semver);
  std::string cxx_version =
      std::format("set(CMAKE_CXX_STANDARD {})", ctx->langversion);
  std::string compiler =
      std::format("set(CMAKE_CXX_COMPILER {})", ctx->compiler);
  std::string source_dir = std::format("set(SOURCE_DIR {})", ctx->src_dir);
  std::string build_dir = std::format("set(BUILD_DIR {})", ctx->build_dir);
  std::string files = "file(GLOB all_SRCS\n\
    \"${PROJECT_SOURCE_DIR}/include/*.h\"\n\
    \"${PROJECT_SOURCE_DIR}/include/*.hpp\"\n\
    \"${PROJECT_SOURCE_DIR}/src/*.cpp\"\n\
    \"${PROJECT_SOURCE_DIR}/src/*.c\"\n\
)";
  std::string include_dir =
      std::format("include_directories({})", ctx->include_dir);
  std::string add_executable =
      std::format("add_executable({} ", ctx->project_name) + "${all_SRCS})";

  std::ofstream file;
  remove("./CMakeLists.txt");

  file.open("./CMakeLists.txt");
  file << cmake_minimum_required << '\n';
  file << project_name << '\n';
  file << cxx_version << '\n';
  file << compiler << '\n';
  file << files << '\n';
  file << include_dir << '\n';
  file << add_executable << '\n';
  file << source_dir << '\n';
  file << build_dir << '\n';
  return false;
}

bool createToml(std::shared_ptr<Context> ctx) {
  std::cout << "Project name: ";
  std::cin >> ctx->project_name;
  std::cout << "src_dir: ";
  std::cin >> ctx->src_dir;
  std::cout << "build_dir: ";
  std::cin >> ctx->build_dir;

  toml::array authors = toml::array{};
  toml::table table = toml::table{
      {"project",
       toml::table{
           {"name", ctx->project_name},
           {"authors", authors},
           {"src_dir", ctx->src_dir},
           {"build_dir", ctx->build_dir},
           {"git", ctx->git},
           {"lang", ctx->lang},
           {"langversion", ctx->langversion},
       }},
  };

  return false;
}
bool createHelloWorld() {
  std::ofstream file;
  file.open("./src/main.cpp");
  file << "#include <iostream>\n\
int main(){\n\
  std::cout << \"Hello World\" << std::endl;\n\
  return 0;\n\
}";
  return false;
}

bool createCppProject(std::shared_ptr<Context> ctx) {

  createToml(ctx);
  loadPackageToml(ctx);
  createCMakelists(ctx);
  return true;
}

bool createCProject() { return false; }

void loadPackageToml(std::shared_ptr<Context> ctx) {
  auto data = toml::parse_file("./config.toml");
  ctx->project_name = data["project"]["name"].value_or("no name");
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
  ctx->langversion = data["project"]["langversion"].value_or("no langversion");
  ctx->semver = data["project"]["semver"].value_or("0.0.1");
};

void savePackageToml(std::shared_ptr<Context> ctx) {
  toml::array authors = toml::array{};
  for (auto &author : ctx->authors) {
    authors.push_back(author);
  }
  toml::table table = toml::table{
      {"project",
       toml::table{
           {"name", ctx->project_name},
           {"authors", authors},
           {"src_dir", ctx->src_dir},
           {"build_dir", ctx->build_dir},
           {"git", ctx->git},
           {"lang", ctx->lang},
           {"langversion", ctx->langversion},
       }},
  };

  std::ofstream file("./build/test.toml");
  file << table;
}

int init(std::shared_ptr<Context> ctx) {
  while (true) {
    if (ctx->lang == "cpp") {
      createCppProject(ctx);
      savePackageToml(ctx);
      break;
    } else if (ctx->lang == "c") {
      createCProject();
      break;
    } else if (ctx->lang == "rust") {
      std::cout << "Fuck off" << ENDL;
      break;
    } else if (ctx->lang == "java") {
      std::cout << "'Are you ok? Stop it get some help' - MJ" << ENDL;
      break;
    } else {
      std::cout << "Invalid language" << ENDL;
      return 1;
    }
  }
  return 0;
}

} 
