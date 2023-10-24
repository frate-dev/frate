cmake_minimum_required(VERSION 3.10)

project ( 
    cmaker
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
FetchContent_Declare(
  json
  GIT_REPOSITORY "https://github.com/nlohmann/json.git"
  GIT_TAG "origin/master"
)

FetchContent_MakeAvailable(json)
FetchContent_MakeAvailable(cxxopts)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR}
  "src/**.cpp"
  "src/**.c"
)

include_directories(${CMAKE_SOURCE_DIR}/include)
set(HEADER_DIR ${CMAKE_SOURCE_DIR}/include)

message("Sources: ${SOURCES}")

#target_link_libraries(cmaker SHARED cxxopts)
include_directories(include)

if(NOT DEFINED RELEASE)
  set(RELEASE 0)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
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

add_executable(cmaker ${SOURCES}) #test mode

if(RELEASE EQUAL 1)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wextra -Wpedantic -Wall")
  add_definitions(-DRELEASE)
else()
  add_definitions(-DDEBUG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wextra -Wpedantic -Wall")
  if(TEST_MODE EQUAL 1)
    FetchContent_Declare(
      Catch2
      GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
      GIT_TAG "origin/devel"
      )

    FetchContent_MakeAvailable(Catch2)
    add_definitions(-DTEST) #test mode
    target_link_libraries(cmaker Catch2::Catch2) #test mode fuck you cmake
    include(CTest)
    include(Catch)

  endif()
endif()

target_link_libraries(cmaker cxxopts)
set(SOURCE_DIR src)
set(BUILD_DIR build)


set_target_properties(cmaker PROPERTIES RUNTIME_OUTPUT_DIRECTORY build)
