#include <string>
#include <nlohmann/json.hpp>
#include "../Command/Command.hpp"
#include <inja.hpp>

namespace Generators::CMakeList {
  bool createCMakeListsExecutable(std::shared_ptr<Command::Project> pro){
  std::cout << "Creating CMakeLists.txt" << std::endl;
  std::cout << pro->toJson() << std::endl;
  std::string CMakeListsExecutable = inja::render(R"EOF(
cmake_minimum_required( VERSION {{ cmake_version }} )
project(
  {{ project_name }}
  VERSION {{ cmake_version }}
  {%if lang == "cpp"%}
  LANGUAGES CXX
)
  {%endif%}
  {%if lang == "c"%}
  LANGUAGES C
)
  {%endif%}

set(CMAKE_CXX_STANDARD {{ lang_version }})
set(CMAKE_CXX_COMPILER {{  compiler }})
include(FetchContent)
##for dep in dependencies
FetchContent_Declare(
  {{  dep.name }}
  GIT_REPOSITORY {{ dep.url }}
  GIT_TAG {{ dep.version }}
)
FetchContent_MakeAvailable({{ dep.name }})
##endfor

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR}
  "src/**.cpp"
  "src/**.c"
  "src/**/**.cpp"
  "src/**/**.c"
)

message("Sources: ${SOURCES}")

include_directories($CMAKE_SOURCE_DIR/{{ include_dir }})
set(HEADER_DIR $CMAKE_SOURCE_DIR/{{ include_dir }})

if(NOT DEFINED RELEASE)
  set(RELEASE 0)
endif()

add_executable({{project_name}} ${SOURCES})

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  message("Release mode")
  set(RELEASE 1)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message("Debug mode")
  set(RELEASE 0)
  set(DEBUG 1)
elseif(CMAKE_BUILD_TYPE STREQUAL "Test")
  message("Test mode")
  set(RELEASE 0)
  set(TEST_MODE 1)
else()
  message("Default mode")
  set(RELEASE 0)
endif()


set(BUILD_DIR {{ build_dir }})
set_target_properties({{project_name}} PROPERTIES RUNTIME_OUTPUT_DIRECTORY {{build_dir}})
if (RELEASE EQUAL 1)
  message("Release mode")
elseif(DEBUG EQUAL 0)
  message("Debug mode")
elseif(TEST_MODE EQUAL 1)
  message("Test mode")
endif()

##for dep in dependencies
target_link_libraries({{project_name}} {{dep.target_link}})
##endfor
install(TARGETS {{project_name}} DESTINATION bin)
)EOF", pro->toJson());
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
  //std::cout << CMakeListsExecutable << std::endl;
  file << CMakeListsExecutable;
  return true;
  }
}
