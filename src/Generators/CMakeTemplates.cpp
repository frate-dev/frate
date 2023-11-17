#include <string>
#include <nlohmann/json.hpp>
#include <CMaker/Command.hpp>
#include <CMaker/Utils/General.hpp>
#include <inja.hpp>

namespace Generators::CMakeList {
  bool createCMakeListsExecutable(std::shared_ptr<Command::Project> pro){
  std::cout << "Creating CMakeLists.txt" << std::endl;
  std::cout << pro->toJson() << std::endl;

  std::string CPM = Utils::fetchText("https://raw.githubusercontent.com/cpm-cmake/CPM.cmake/v0.38.6/cmake/CPM.cmake");
  std::ofstream CPMFile;
  try{
    if(!std::filesystem::exists(pro->project_path / "cmake"))
      std::filesystem::create_directory(pro->project_path / "cmake");
    CPMFile.open(pro->project_path / "cmake/CPM.cmake");
  }catch(...){
    std::cout << "Error while opening file: CPM.cmake" << std::endl;
    return false;
  }
  CPMFile << CPM;
  std::string CMakeListsExecutable = inja::render(R"EOF(
cmake_minimum_required( VERSION {{ cmake_version }} )
project(
  {{ project_name }}
  VERSION {{ project_version }}
  {%if lang == "cpp"%}
  LANGUAGES CXX
)
  {%endif%}
  {%if lang == "c"%}
  LANGUAGES C
)
  {%endif%}

set(CMAKE_CXX_STANDARD {{ lang_version }})
include(cmake/CPM.cmake)

CPMAddPackage(
  NAME Ccache.cmake
  GITHUB_REPOSITORY TheLartians/Ccache.cmake
  VERSION 1.2
)
include(FetchContent)
##for dep in dependencies
#FetchContent_Declare(
#  {{  dep.name }}
#  GIT_REPOSITORY {{ dep.git }}
#  GIT_TAG {{ dep.version }}
#)
CPMAddPackage(
  NAME {{ dep.name }}
  GIT_REPOSITORY {{ dep.git }}
  GIT_TAG {{ dep.version }})
#FetchContent_MakeAvailable({{ dep.name }})
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
##for mode in modes
if (CMAKE_BUILD_TYPE STREQUAL "{{ mode.name }}")
  add_definitions(-D{{ mode.name }})
  {% for dep in mode.dependencies %}
  CPMAddPackage(
    NAME {{ dep.name }}
    GIT_REPOSITORY {{ dep.git }}
    GIT_TAG {{ dep.version }}
  )
  {% endfor %}
  set_target_properties({{project_name}} PROPERTIES COMPILE_FLAGS "{% for flag in mode.flags %} {{ flag }} {% endfor %}")
endif()
##endfor
set(BUILD_DIR {{ build_dir }})
set_target_properties({{project_name}} PROPERTIES RUNTIME_OUTPUT_DIRECTORY {{build_dir}})


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




