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
  bool createCMakelists(std::shared_ptr<Context> ctx) {
    std::string cmake_minimum_required =
      std::format("cmake_minimum_required(VERSION {})", ctx->cmake_version);
    std::string project_name =
      std::format("project ({} VERSION {} LANGUAGES CXX)",
          ctx->project_name, ctx->project_version);
    std::string build_type = R"V0G0N(
      if (CMAKE_BUILD_TYPE STREQUAL "Release")
        message("Release mode")
        set(RELEASE 1)
      elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        message("Debug mode")
        set(RELEASE 0)
      elseif(CMAKE_BUILD_TYPE STREQUAL "Test")
        message("Test mode")
        set(RELEASE 0)
        set(TEST_MODE 1)
      else()
        message("Default mode")
        set(RELEASE 0)
      endif()
      )V0G0N";
    std::string cxx_version =
      std::format("set(CMAKE_CXX_STANDARD {})", ctx->lang_version);
    std::string compiler =
      std::format("set(CMAKE_CXX_COMPILER {})\n", ctx->compiler);
    std::string source_dir = std::format("set(SOURCE_DIR {})", ctx->src_dir);
    std::string build_dir = std::format("set(BUILD_DIR {})", ctx->build_dir);
    std::string FetchContent = "include(FetchContent)";

    std::string files = R"V0G0N(
      file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR}
        "include/**.h"
        "include/**.hpp"
        "src/**.cpp"
        "src/**.c"
        )
      )V0G0N";
    typedef struct make_dep {
      std::string fetch_declare;
      std::string fetch_make_available;
      std::string target_link_libraries;
    } make_dep;

    std::vector<make_dep> dependencies;
    for (Command::dependency &dep : ctx->dependencies) {
      std::string FetchContent_Declare =
        std::format(
        R"V0G0N(
        FetchContent_Declare(
          {}
          GIT_REPOSITORY "{}"
          GIT_TAG "{}"
        )
        )V0G0N",
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
    std::string include_fetch = "include(FetchContent)";

    std::string include_dir =
      std::format("include_directories({})", ctx->include_dir);
    std::string add_executable =
      std::format("add_executable({} ", ctx->project_name) + "${SOURCES})";
    std::string testing = "ok";
    std::string mode = R"V0G0N(
      if(RELEASE EQUAL 1)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wextra -Wpedantic -Wall")
        add_definitions(-DRELEASE)
      else()
        add_definitions(-DDEBUG)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wextra -Wpedantic -Wall")
        if(TEST_MODE EQUAL 1)
        endif()
      endif()
      )V0G0N";
    std::string set_build_dir = std::format(
        "set_target_properties({} PROPERTIES RUNTIME_OUTPUT_DIRECTORY {})",
        ctx->project_name, ctx->build_dir);

    std::ofstream file;
    std::string file_name = "CMakeLists.txt";

#ifdef DEBUG
    file_name = "build/CMakeLists.txt";
#endif
    remove((ctx->project_path / file_name).c_str());

    file.open(ctx->project_path / file_name);
    file << cmake_minimum_required << '\n';
    file << project_name << '\n';
    file << cxx_version << '\n';
    file << build_type << '\n';
    file << files << '\n';
    file << add_executable << '\n';
    file << FetchContent << '\n';
    for (make_dep &dep : dependencies) {
      file << dep.fetch_declare << '\n';
      file << dep.fetch_make_available << '\n';
      file << dep.target_link_libraries << '\n';
    }
    file << mode << '\n';
    file << include_dir << '\n';
    file << source_dir << '\n';
    file << build_dir << '\n';
    file << set_build_dir << '\n';
    return false;
  }
} // namespace Command
