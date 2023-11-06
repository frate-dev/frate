cmake_minimum_required(VERSION 3.10)

project ( 
    cmaker
    VERSION 0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_CURL_EXE OFF)
include(FetchContent)

FetchContent_Declare(
  cxxopts 
  GIT_REPOSITORY "https://github.com/jarro2783/cxxopts.git"
  GIT_TAG "origin/master"
)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY "https://github.com/nlohmann/json.git"
  GIT_TAG "origin/master"
)

FetchContent_Declare(
  curl
  GIT_REPOSITORY "https://github.com/curl/curl.git"
  GIT_TAG "curl-8_3_0"
)

FetchContent_Declare(
  termcolor
  GIT_REPOSITORY "https://github.com/ikalnytskyi/termcolor.git"
  GIT_TAG "v2.1.0"
)

FetchContent_MakeAvailable(nlohmann_json)
FetchContent_MakeAvailable(cxxopts)
FetchContent_MakeAvailable(curl)
FetchContent_MakeAvailable(termcolor)

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
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wextra -Wpedantic -Wall -fprofile-arcs -ftest-coverage")
  if(TEST_MODE EQUAL 1)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wextra -Wpedantic -Wall")
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

target_link_libraries(${PROJECT_NAME} libcurl)
target_link_libraries(${PROJECT_NAME} cxxopts)
target_link_libraries(${PROJECT_NAME} nlohmann_json)
target_link_libraries(${PROJECT_NAME} termcolor::termcolor)

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
