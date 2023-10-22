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
)

include_directories(${CMAKE_SOURCE_DIR}/include)
set(HEADER_DIR ${CMAKE_SOURCE_DIR}/include)

message("Sources: ${SOURCES}")

#target_link_libraries(cmake-generator SHARED cxxopts)
include_directories(include)
add_executable(cmake-generator ${SOURCES})

if(NOT DEFINED RELEASE)
  set(RELEASE 0)
endif()

if(RELEASE EQUAL 1)
  message("Release mode")
  add_definitions(-DRELEASE)
else()
  message("Debug mode")
  add_definitions(-DDEBUG)
endif()

target_link_libraries(cmake-generator cxxopts)
set(SOURCE_DIR src)
set(BUILD_DIR build)


set_target_properties(cmake-generator PROPERTIES RUNTIME_OUTPUT_DIRECTORY build)
