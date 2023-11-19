cmake_minimum_required(VERSION 3.10)

project ( 
    cmaker
    VERSION 0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(-DUSE_CCACHE=YES -DCCACHE_OPTIONS="CCACHE_CPP2=true;CCACHE_SLOPPINESS=clang_index_store")


set(BUILD_CURL_EXE OFF)
include (cmake/CPM.cmake)
CPMUsePackageLock(package-lock.cmake)
CPMAddPackage(
  NAME Ccache.cmake
  GITHUB_REPOSITORY TheLartians/Ccache.cmake
  VERSION 1.2
)
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
  NAME curl
  GITHUB_REPOSITORY curl/curl
  GIT_TAG curl-8_3_0
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

add_executable(cmaker ${SOURCES}) #test mode

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  message("Release mode")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wextra -Wpedantic -Wall")
  add_definitions(-DRELEASE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message("Debug mode")
  add_definitions(-DDEBUG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wextra -Wpedantic -Wall -Wredundant-decls -Wunused -Wunused-parameter -Wunused-function -Wunused-macros")
elseif(CMAKE_BUILD_TYPE STREQUAL "Test")
  message("Test mode")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wextra -Wpedantic -Wall")
  CPMAddPackage("gh:catchorg/Catch2@3.4.0")
  add_definitions(-DTEST -DDEBUG) #test mode
  target_link_libraries(cmaker Catch2::Catch2WithMain) #test mode fuck you cmake
else()
  add_definitions(-DDEBUG)
  message("Default mode")
endif()

target_link_libraries(${PROJECT_NAME} libcurl)
target_link_libraries(${PROJECT_NAME} cxxopts)
target_link_libraries(${PROJECT_NAME} nlohmann_json)
target_link_libraries(${PROJECT_NAME} termcolor)

set(SOURCE_DIR src)
set(BUILD_DIR build)
set_target_properties(cmaker PROPERTIES RUNTIME_OUTPUT_DIRECTORY build)
install(TARGETS cmaker DESTINATION bin)
install(FILES ./completions/cmaker-completion.zsh
        DESTINATION /usr/local/share/zsh/site-functions/
        RENAME _cmaker)
install(FILES ./completions/cmaker-completion.bash
        DESTINATION /etc/bash_completion.d/
        RENAME cmaker)
install(FILES ./completions/cmaker-completion.fish
    DESTINATION /usr/share/fish/vendor_completions.d
    RENAME cmaker.fish)

if(NOT DEFINED MAN_INSTALL_DIR)
  set(MAN_INSTALL_DIR "share/man/man1")
endif()

add_custom_target(man ALL DEPENDS docs/man/cmaker.1)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/docs/man/cmaker.1" DESTINATION "${MAN_INSTALL_DIR}")
