#include "Command.h"
#include "toml.hpp"
#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <string>

namespace Command {

bool createCMakelists(std::shared_ptr<Context> ctx) {
  std::string cmake_minimum_required =
      std::format("cmake_minimum_required(VERSION {})", ctx->cmake_version);
  std::string project_name =
      std::format("project ( \n\
    {} \n\
    VERSION {}\n\
    LANGUAGES CXX\n\
)",
                  ctx->project_name, ctx->project_version);

  std::string cxx_version =
      std::format("set(CMAKE_CXX_STANDARD {})", ctx->lang_version);
  std::string compiler =
      std::format("set(CMAKE_CXX_COMPILER {})", ctx->compiler);
  std::string source_dir = std::format("set(SOURCE_DIR {})", ctx->src_dir);
  std::string build_dir = std::format("set(BUILD_DIR {})", ctx->build_dir);
  std::string files = "file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR}\n\
    \"include/**.h\"\n\
    \"include/**.hpp\"\n\
    \"src/**.cpp\"\n\
    \"src/**.c\"\n\
)";
  std::string include_dir =
      std::format("include_directories({})", ctx->include_dir);
  std::string add_executable =
      std::format("add_executable({} ", ctx->project_name) + "${SOURCES})";

  std::string set_build_dir = std::format(
      "set_target_properties({} PROPERTIES RUNTIME_OUTPUT_DIRECTORY {})",
      ctx->project_name, ctx->build_dir);

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
  file << set_build_dir << '\n';
  return false;
}

bool createToml(std::shared_ptr<Context> ctx) {
  std::string cmake_version;
  std::string project_name;
  std::string project_version;
  std::string include_dir;
  std::string compiler;
  std::string name;
  std::string src_dir;
  std::string build_dir;
  std::string lang;
  std::string lang_version;
  std::string authors_str;

  std::cout << "🔨Cmake version: (" << ctx->cmake_version << "): ";
  std::getline(std::cin, cmake_version);
  std::cout << "📖Project name: (" << ctx->project_name << "): ";
  std::getline(std::cin, project_name);
  std::cout << "🗃️Version: (" << ctx->project_version << "): ";
  std::getline(std::cin, project_version);
  std::cout << "📰Language Standard: (" << ctx->lang_version << "): ";
  std::getline(std::cin, lang_version);
  std::cout << "💽Compiler: (" << ctx->compiler << "): ";
  std::getline(std::cin, compiler);
  std::cout << "⛲Source Dir: (" << ctx->src_dir << "): ";
  std::getline(std::cin, src_dir);
  std::cout << "🛠️Build Dir: (" << ctx->build_dir << "): ";
  std::getline(std::cin, build_dir);
  std::cout << "🫃Include Dir: (" << ctx->include_dir << "): ";
  std::getline(std::cin, include_dir);

  ctx->cmake_version = cmake_version == "" ? ctx->cmake_version : cmake_version;
  ctx->project_name = project_name == "" ? ctx->project_name : project_name;
  ctx->project_version =
      project_version == "" ? ctx->project_version : project_version;
  ctx->lang_version = lang_version == "" ? ctx->lang_version : lang_version;
  ctx->lang = lang == "" ? ctx->lang : lang;
  ctx->compiler = compiler == "" ? ctx->compiler : compiler;
  ctx->src_dir = src_dir == "" ? ctx->src_dir : src_dir;
  ctx->build_dir = build_dir == "" ? ctx->build_dir : build_dir;
  ctx->include_dir = include_dir == "" ? ctx->include_dir : include_dir;

  toml::array authors = toml::array{};
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
       }},
  };
  std::cout << "📄New Toml File: \n";
  std::cout << table << '\n';

  std::ofstream file;
  file.open("./config.toml");
  file << table;
  file << '\n';
  file.close();
  return false;
}

bool createHelloWorldCpp() {
  system("cd build;mkdir src");
  std::ofstream file;
  file.open("./build/src/main.cpp");
  file << "#include <iostream>\n"
          "int main(){\n"
          " std::cout << \"Hello World\" << std::endl;\n"
          " return 0;\n"
          "}\n";
  return false;
}
bool createHelloWorldC() {
  system("mkdir src");
  std::ofstream file;
  file.open("./src/main.c");
  file << "#include <stdio.h>\n"
          "int main(){\n"
          " printf(\"Hello World\");\n"
          " return 0;\n"
          "}\n";
  return false;
}
bool createCppProject(std::shared_ptr<Context> ctx) {

  createToml(ctx);
  loadPackageToml(ctx);
  createCMakelists(ctx);
  createHelloWorldCpp();
  return true;
}

bool createCProject(std::shared_ptr<Context> ctx) {
  createToml(ctx);
  loadPackageToml(ctx);
  createCMakelists(ctx);
  createHelloWorldC();
  return false;
}

bool defaultTomlCpp(std::shared_ptr<Context> ctx) {
  ctx->project_name = "TestProject";
  ctx->src_dir = "./src";
  ctx->build_dir = "./build";

  toml::array authors = toml::array{};
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
       }},
  };
  std::cout << "📄New Toml File: \n";
  std::cout << table << '\n';

  std::ofstream file;
  std::string file_name = "./config.toml";
  #ifdef DEBUG
     file_name = "./build/config.toml";
  #endif

  file.open(file_name);
  file << table;
  file << '\n';
  file.close();
  return false;
}

bool init(std::shared_ptr<Context> ctx, std::vector<std::string> &args) {

  if (std::find(args.begin(), args.end(), "-y") != args.end()) {
    defaultTomlCpp(ctx);
    loadPackageToml(ctx);
    createCMakelists(ctx);
    createHelloWorldCpp();
  } else {
    while (true) {
      std::cout << "Language: ";
      std::getline(std::cin, ctx->lang);
      if (ctx->lang == "cpp" || ctx->lang == "c++") {
        createCppProject(ctx);
        break;
      } else if (ctx->lang == "c") {
        createCProject(ctx);
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
  }

  return true;
}

} // namespace Command
