#include "Command.hpp"
#include "toml.hpp"
#include <cstddef>
#include <format>
#include <iostream>

namespace Command {
bool loadPackageToml(std::shared_ptr<Context> ctx) {
  try {
    std::string file_name = "./config.toml";
#ifdef DEBUG
    file_name = "./build/config.toml";
#endif

    auto data = toml::parse_file(file_name);
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
    /* for (auto &flag : *data["project"]["cflags"].as_array()) { */
    /* ctx->flags.push_back(flag.value_or("no flag")); */
    /* } */

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
  std::ofstream file;
  std::string file_name = "./config.toml";
#ifdef DEBUG
  file_name = "./build/config.toml";
#endif
  file.open(file_name);
  file << table;
  file << '\n';
  file.close();
  return true;
}
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
  std::string build_type =
      "if (CMAKE_BUILD_TYPE STREQUAL \"Release\")\n\tmessage(\"Release "
      "mode\")\n\tset(RELEASE 1)\n\telseif(CMAKE_BUILD_TYPE STREQUAL "
      "\"Debug\")\n\tmessage(\"Debug mode\")\n\tset(RELEASE "
      "0)\nelseif(CMAKE_BUILD_TYPE STREQUAL \"Test\")\n\tmessage(\"Test "
      "mode\")\n\tset(RELEASE 0)\n\tset(TEST_MODE 1)\nelse()\n";
  std::string cxx_version =
      std::format("set(CMAKE_CXX_STANDARD {})", ctx->lang_version);
  std::string compiler =
      std::format("set(CMAKE_CXX_COMPILER {})\n", ctx->compiler);
  std::string source_dir = std::format("set(SOURCE_DIR {})", ctx->src_dir);
  std::string build_dir = std::format("set(BUILD_DIR {})", ctx->build_dir);
  std::string FetchContent = "include(FetchContent)";
  std::string files =
      "file(GLOB_RECURSE SOURCES RELATIVE "
      "${CMAKE_SOURCE_DIR}\n\t\"include/**.h\"\n "
      "\"include/**.hpp\"\n\t\"src/**.cpp\"\n\t\"src/**.c\"\n)";
  typedef struct make_dep {
    std::string fetch_declare;
    std::string fetch_make_available;
    std::string target_link_libraries;
  } make_dep;

  std::vector<make_dep> dependencies;
  for (Command::dependency &dep : ctx->dependencies) {
    std::string FetchContent_Declare =
        std::format("FetchContent_Declare(\n\t{} \n\tGIT_REPOSITORY "
                    "\"{}\"\n\tGIT_TAG \"{}\"\n)",
                    dep.name, dep.url, dep.version);
    std::string FetchContent_MakeAvailable =
        std::format("FetchContent_MakeAvailable({})", dep.name);
    std::string target_link_libraries = std::format(
        "target_link_libraries({} {})", ctx->project_name, dep.name);
    make_dep dependency =
        make_dep{FetchContent_Declare, FetchContent_MakeAvailable,
                 target_link_libraries};
    dependencies.push_back(dependency);
  }

  std::string include_dir =
      std::format("include_directories({})", ctx->include_dir);
  std::string add_executable =
      std::format("add_executable({} ", ctx->project_name) + "${SOURCES})";
  std::string testing="ok";
  //std::string mode = std::format("if(RELEASE EQUAL 1)\n\tset(CMAKE_CXX_FLAGS {} \"${CMAKE_CXX_FLAGS} -O3 -Wextra -Wpedantic -Wall\")\n\tadd_definitions(-DRELEASE)\nelse()\n\tadd_definitions(-DDEBUG)\n\tset(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} -g -O0 -Wextra {} -Wpedantic -Wall\")\n\tif(TEST_MODE EQUAL 1)\n\tendif()\nendif()", testing);
  std::string set_build_dir = std::format(
      "set_target_properties({} PROPERTIES RUNTIME_OUTPUT_DIRECTORY {})",
      ctx->project_name, ctx->build_dir);

  std::ofstream file;
  std::string file_name = "./CMakeLists.txt";

#ifdef DEBUG
  file_name = "./build/CMakeLists.txt";
#endif
  remove(file_name.c_str());

  file.open(file_name);
  file << cmake_minimum_required << '\n';
  file << project_name << '\n';
  file << cxx_version << '\n';
  file << compiler << '\n';
  file << build_type << '\n';
  for (make_dep &dep : dependencies) {
    file << dep.fetch_declare << '\n';
    file << dep.fetch_make_available << '\n';
    file << dep.target_link_libraries << '\n';
  }
  file << files << '\n';
  file << include_dir << '\n';
  file << add_executable << '\n';
  file << source_dir << '\n';
  file << build_dir << '\n';
  file << set_build_dir << '\n';
  return false;
}
} // namespace Command
