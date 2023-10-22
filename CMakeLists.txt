cmake_minimum_required(VERSION 3.10)
project ( 
    cmake-generator 
    VERSION 0.0.1
    LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 20)


include(FetchContent)

FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY "https://github.com/jarro2783/cxxopts.git"
  GIT_TAG "origin/master"
)


FetchContent_MakeAvailable(cxxopts)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR}
  "src/**.cpp"
  "src/**.c"
  #"${PROJECT_SOURCE_DIR}/include/*.h"
  #"${PROJECT_SOURCE_DIR}/include/*.hpp"
)

include_directories(${CMAKE_SOURCE_DIR}/include)
set(HEADER_DIR ${CMAKE_SOURCE_DIR}/include)

message("Sources: ${SOURCES}")

#target_link_libraries(cmake-generator SHARED cxxopts)
include_directories(include)
add_executable(cmake-generator ${SOURCES})
target_link_libraries(cmake-generator cxxopts)
set(SOURCE_DIR src)
set(BUILD_DIR build)


set_target_properties(cmake-generator PROPERTIES RUNTIME_OUTPUT_DIRECTORY build)
