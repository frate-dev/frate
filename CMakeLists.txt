cmake_minimum_required(VERSION 3.10)

set(VMINOR 0)
set(VMAJOR 0)
set(VPATCH 1)



if(DEFINED ENV{VBUILD})
  set(VBUILD $ENV{VBUILD})
else()
  set(VBUILD 0)
endif()

add_definitions(-DVMINOR=${VMINOR})
add_definitions(-DVMAJOR=${VMAJOR})
add_definitions(-DVPATCH=${VPATCH})
add_definitions(-DVBUILD=${VBUILD})


set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_C_COMPILER /usr/bin/clang)
project ( 
    frate
    VERSION ${VMAJOR}.${VMINOR}.${VPATCH}.${VBUILD}
    LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 20)
set(-DUSE_CCACHE=YES -DCCACHE_OPTIONS="CCACHE_CPP2=true;CCACHE_SLOPPINESS=clang_index_store")

find_program(SCCACHE_FOUND sccache)
find_program(CCACHE_FOUND ccache)

find_library(LUA_FOUND lua)



set(SCCACHE_FOUND 0)

if(SCCACHE_FOUND)
  message("Using sccache")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE sccache)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK sccache)
  set(CMAKE_CXX_COMPILER_LAUNCHER sccache)
  set(CMAKE_C_COMPILER_LAUNCHER sccache)
elseif(CCACHE_FOUND)
  message("Using ccache")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
  set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
  set(CMAKE_C_COMPILER_LAUNCHER ccache)
else()
  message("No cache found")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


include (cmake/CPM.cmake)


CPMAddPackage(
  NAME termcolor
  GITHUB_REPOSITORY ikalnytskyi/termcolor
  GIT_TAG v2.1.0
)

CPMAddPackage(
  NAME cxxopts
  GITHUB_REPOSITORY jarro2783/cxxopts
  GIT_TAG origin/master
)
CPMAddPackage(
  NAME nlohmann_json
  GITHUB_REPOSITORY nlohmann/json
  GIT_TAG origin/master
)
CPMAddPackage(
  name libgit2
  GITHUB_REPOSITORY libgit2/libgit2
  GIT_TAG v1.7.1
  OPTIONS
    "BUILD_TESTS OFF"
    "BUILD_CLI OFF"
)
CPMAddPackage(
  NAME curl
  GITHUB_REPOSITORY curl/curl
  GIT_TAG curl-8_3_0
  OPTIONS
    "ENABLE_VERSIONED_SYMBOLS ON"
)

CPMAddPackage(
  NAME uv 
  GITHUB_REPOSITORY libuv/libuv
  GIT_TAG v1.47.0
)

CPMAddPackage(
  lua
  GITHUB_REPOSITORY lua/lua
  GIT_TAG v5.4.4
)

CPMAddPackage(
  NAME sol2
  GITHUB_REPOSITORY ThePhD/sol2
  GIT_TAG v3.3.1
)


file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR}
  "src/**.cpp"
  "src/**.c"
  "src/**/**.cpp"
  "src/**/**.c"
)

include_directories(${CMAKE_SOURCE_DIR}/include)
set(HEADER_DIR ${CMAKE_SOURCE_DIR}/include)

message("Sources: ${SOURCES}")



if(NOT DEFINED RELEASE)
  set(RELEASE 0)
endif()
add_executable(${PROJECT_NAME} ${SOURCES}) #test mode

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  message("Release mode")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O2 -Wextra -Wpedantic -Wall")
  add_definitions(-DRELEASE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message("Debug mode")
  add_definitions(-DDEBUG)
  CPMAddPackage("gh:jeremy-rifkin/cpptrace@0.2.1")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -g -O0 -Wextra -Wpedantic -Wall -Wredundant-decls -Wunused -Wunused-parameter -Wunused-function -Wunused-macros")
  target_link_libraries(${PROJECT_NAME} cpptrace)
elseif(CMAKE_BUILD_TYPE STREQUAL "Test")
  message("Test mode")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -g -O0 -Wextra -Wpedantic -Wall")
  CPMAddPackage("gh:catchorg/Catch2@3.4.0")
  CPMAddPackage("gh:jeremy-rifkin/cpptrace@0.2.1")
  add_definitions(-DTEST -DDEBUG) #test mode
  target_link_libraries(${PROJECT_NAME} Catch2::Catch2WithMain) #test mode fuck you cmake
  target_link_libraries(${PROJECT_NAME} cpptrace)
else()
  add_definitions(-DDEBUG)
  message("Default mode")
endif()
target_link_libraries(${PROJECT_NAME} libcurl)
target_link_libraries(${PROJECT_NAME} cxxopts)
target_link_libraries(${PROJECT_NAME} nlohmann_json)
target_link_libraries(${PROJECT_NAME} libgit2package)
target_link_libraries(${PROJECT_NAME} uv)
target_link_libraries(${PROJECT_NAME} termcolor)
target_link_libraries(${PROJECT_NAME} lua)
target_link_libraries(${PROJECT_NAME} sol2::sol2)

set(SOURCE_DIR src)
set(BUILD_DIR build)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY build)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES ./completions/${PROJECT_NAME}-completion.zsh
        DESTINATION /usr/local/share/zsh/site-functions/
        RENAME _${PROJECT_NAME})
      install(FILES ./completions/${PROJECT_NAME}-completion.bash
        DESTINATION /etc/bash_completion.d/
        RENAME ${PROJECT_NAME})
      install(FILES ./completions/${PROJECT_NAME}-completion.fish
    DESTINATION /usr/share/fish/vendor_completions.d
    RENAME ${PROJECT_NAME}.fish)

if(NOT DEFINED MAN_INSTALL_DIR)
  set(MAN_INSTALL_DIR "share/man/man1")
endif()

add_custom_target(man ALL DEPENDS docs/man/${PROJECT_NAME}.1)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/docs/man/${PROJECT_NAME}.1" DESTINATION "${MAN_INSTALL_DIR}")
