#include "Generators.hpp"
#include <memory>
#include <inja.hpp>
#include <vector>
#include <format>
#include "../Command/Command.hpp"



namespace Generators::CMakeList{
  /*
   * Generate the dependencies for the project
   * @param ctx: the context of the command
   * @return a vector of dependencies that will be later combined to build the cmake file
   */
  void generateDeps(std::shared_ptr<Command::Project> pro, std::shared_ptr<CMakeContext> cmake_context){
    for (auto dep : pro->dependencies) {
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
          "target_link_libraries({} {})", pro->project_name, dep.target_link);
      Dep dependency =
        Dep{FetchContent_Declare, FetchContent_MakeAvailable,
          target_link_libraries};
      std::cout << dependency.fetch_declare << std::endl;
      std::cout << dependency.fetch_make_available << std::endl;
      std::cout << dependency.target_link_libraries << std::endl;
      cmake_context->dependencies.push_back(dependency);
    }
  }
  bool create(std::shared_ptr<Command::Project> pro) {
    std::shared_ptr<CMakeContext> cmake_context = std::make_shared<CMakeContext>();
    cmake_context->cmake_minimum_required =
      std::format("cmake_minimum_required(VERSION {})", pro->cmake_version);
    cmake_context->project_name =
      std::format("project ({} VERSION {} LANGUAGES CXX)",
          pro->project_name, pro->project_version);
    cmake_context->build_type = R"(
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
      )";
    cmake_context->cxx_version =
      std::format("set(CMAKE_CXX_STANDARD {})", pro->lang_version);
    cmake_context->compiler =
      std::format("set(CMAKE_CXX_COMPILER {})\n", pro->compiler);
    cmake_context->source_dir = std::format("set(SOURCE_DIR {})", pro->src_dir);
    cmake_context->build_dir = std::format("set(BUILD_DIR {})", pro->build_dir);
    cmake_context->fetch_content = "include(FetchContent)";

    cmake_context->files = R"V0G0N(
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR}
  "include/**.h"
  "include/**.hpp"
  "src/**.cpp"
  "src/**.c"
  )
      )V0G0N";

    cmake_context->include_fetch = "include(FetchContent)";

    generateDeps(pro, cmake_context);

    cmake_context->include_dir =
      std::format("include_directories({})", pro->include_dir);
    cmake_context->add_executable =
      std::format("add_executable({} ", pro->project_name) + "${SOURCES})";
    cmake_context->testing = "ok";
    cmake_context->mode = R"V0G0N(
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
    cmake_context->set_build_dir = std::format(
        "set_target_properties({} PROPERTIES RUNTIME_OUTPUT_DIRECTORY {})",
        pro->project_name, pro->build_dir);

    std::ofstream file;
    std::string file_name = "CMakeLists.txt";

    try{
      remove((pro->project_path / file_name).c_str());
    }catch(...){
      std::cout << "Error while removing file: " << file_name << std::endl;
      return false;
    }

    try{
      file.open(pro->project_path / file_name);
    }catch(...){
      std::cout << "Error while opening file: " << file_name << std::endl;
      return false;
    }
    file << cmake_context->cmake_minimum_required << '\n';
    file << cmake_context->project_name << '\n';
    file << cmake_context->cxx_version << '\n';
    file << cmake_context->build_type << '\n';
    file << cmake_context->files << '\n';
    file << cmake_context->add_executable << '\n';
    file << cmake_context->fetch_content << '\n';
    for (Dep dep : cmake_context->dependencies) {
      file << dep.fetch_declare << '\n';
      file << dep.fetch_make_available << '\n';
      file << dep.target_link_libraries << '\n';
    }
    file << cmake_context->mode << '\n';
    file << cmake_context->include_dir << '\n';
    file << cmake_context->source_dir << '\n';
    file << cmake_context->build_dir << '\n';
    file << cmake_context->set_build_dir << '\n';
    return true;
  }
}
