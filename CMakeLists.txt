cmake_minimum_required(VERSION 3.10)

set(VMINOR 0)
set(VMAJOR 0)
set(VPATCH 1)


if(DEFINED ENV{VBUILD})
  set(VBUILD $ENV{VBUILD})
else()
  set(VBUILD 0)
endif()
#set(CMAKE_EXE_LINKER_FLAGS "-static")

set(BRANCH "main")
find_package(Git QUIET)
if(GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  set(BRANCH ${BRANCH})
endif()
message("${BRANCH}")

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
add_definitions(-DVMINOR=${VMINOR})
add_definitions(-DVMAJOR=${VMAJOR})
add_definitions(-DVPATCH=${VPATCH})
add_definitions(-DVBUILD=${VBUILD})
add_definitions(-DBRANCH_MAIN=1)
if(BRANCH STREQUAL "dev")
  remove_definitions(-DBRANCH_MAIN=1)
  add_definitions(-DBRANCH_DEV=1)
endif()

#set(CMAKE_CXX_COMPILER /usr/bin/clang++)
#set(CMAKE_C_COMPILER /usr/bin/clang)
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
  GIT_REPOSITORY https://github.com/ikalnytskyi/termcolor
  GIT_TAG v2.1.0
  OPTIONS
  "BUILD_STATIC_LIBS ON"
)

CPMAddPackage(
  NAME cxxopts
  GIT_REPOSITORY https://www.github.com/jarro2783/cxxopts
  GIT_TAG origin/master
  OPTIONS
  "BUILD_STATIC_LIBS ON"

)
CPMAddPackage(
  NAME nlohmann_json
  GIT_REPOSITORY https://www.github.com/nlohmann/json
  GIT_TAG v3.11.3
  OPTIONS
  "BUILD_STATIC_LIBS ON"
  "JSON_BuildTests OFF"
  "JSON_Diagnostics ON"


)

CPMAddPackage(
  NAME curl
  GIT_REPOSITORY https://www.github.com/curl/curl
  GIT_TAG curl-8_3_0
  OPTIONS
    "ENABLE_VERSIONED_SYMBOLS ON"
    "BUILD_CURL_EXE OFF"
    "BUILD_STATIC_LIBS ON"
    "BUILD_STATIC_CURL  ON"
    "ENABLE_STATIC ON"
    "CURL_STATICLIB ON"
    "WITH_SSL ON"
    "WITH_ZLIB ON"
    "LIBSSH2_LIBRARY OFF"
    "CURL_DISABLE_AWS ON"
    "CURL_DISABLE_FTP ON"
    "CURL_DISABLE_POP3 ON"
)

CPMAddPackage(
  NAME uv 
  GIT_REPOSITORY https://www.github.com/libuv/libuv
  GIT_TAG v1.47.0
  OPTIONS
    "BUILD_STATIC_LIBS ON"
    "LIBUV_BUILD_SHARED OFF"
    "LIBUV_STATIC_LIBRARY O"
)

CPMAddPackage(
  NAME lua
  GIT_REPOSITORY https://www.github.com/lua/lua
  GIT_TAG v5.4.4
  OPTIONS
    "BUILD_STATIC_LIBS ON"
)  

CPMAddPackage(
  NAME sol2
  GIT_REPOSITORY https://www.github.com/ThePhD/sol2
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
  CPMAddPackage(
    "gh:jeremy-rifkin/cpptrace@0.2.1"
  )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -g -O0 -Wextra -Wpedantic -Wall -Wredundant-decls -Wunused -Wunused-parameter -Wunused-function -Wunused-macros -fsanitize=address -fno-omit-frame-pointer")
  target_link_libraries(${PROJECT_NAME} PRIVATE cpptrace)
elseif(CMAKE_BUILD_TYPE STREQUAL "Test")
  message("Test mode")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -g -O0 -Wextra -Wpedantic -Wall -fno-omit-frame-pointer")
  CPMAddPackage(
    "gh:catchorg/Catch2@3.4.0"
  )
  CPMAddPackage(
    "gh:jeremy-rifkin/cpptrace@0.2.1"
  )
  add_definitions(-DTEST -DDEBUG) #test mode
  target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain) #test mode fuck you cmake
  target_link_libraries(${PROJECT_NAME} PRIVATE cpptrace)
else()
  add_definitions(-DDEBUG)
  message("Default mode")
endif()

target_link_options(${PROJECT_NAME} PRIVATE   -static-libgcc -static-libstdc++)
#target_link_libraries(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
target_link_libraries(${PROJECT_NAME} PRIVATE libcurl)
target_link_libraries(${PROJECT_NAME} PRIVATE cxxopts)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE uv)
target_link_libraries(${PROJECT_NAME} PRIVATE termcolor)
target_link_libraries(${PROJECT_NAME} PRIVATE lua)
target_link_libraries(${PROJECT_NAME} PRIVATE sol2::sol2)

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
set(CPACK_PACKAGE_VENDOR "Frate")
if ( DIST_UBUNTU )
  set(CPACK_GENERATOR "DEB")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Lucas Sproule & Jon-Michael Hartway")
  set(CPACK_PACKAGE_VERSION_MAJOR ${VMAJOR})
  set(CPACK_PACKAGE_VERSION_MINOR ${VMINOR})
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "build-essential, cmake, git,  ccache,  libssh2-1,  lua5.4, liblua5.4-dev")
  set(CPACK_PACKAGE_DESCRIPTION "Frate a package/build system manager for C/C++")
  set(CPACK_PACKAGE_CONTACT "Lucas Sproule frate@frate.dev")
endif()
if(DIST_TGZ)
  set(CPACK_GENERATOR "TGZ")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Lucas Sproule & Jon-Michael Hartway")
  set(CPACK_PACKAGE_VERSION_MAJOR ${VMAJOR})
  set(CPACK_PACKAGE_VERSION_MINOR ${VMINOR})
  #set(CPACK_DEBIAN_PACKAGE_DEPENDS "build-essential, cmake, git,  ccache,  libssh2-1,  lua5.4, liblua5.4-dev")
  set(CPACK_PACKAGE_DESCRIPTION "Frate a package/build system manager for C/C++")
  set(CPACK_PACKAGE_CONTACT "Lucas Sproule frate@frate.dev")
endif()
if(DIST_RPM)
  set(CPACK_GENERATOR "RPM")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Lucas Sproule & Jon-Michael Hartway")
  set(CPACK_PACKAGE_VERSION_MAJOR ${VMAJOR})
  set(CPACK_PACKAGE_VERSION_MINOR ${VMINOR})
  set(CPACK_RPM_BUILDREQUIRES "autoconf, automake, clang, libssh, binutils, bison, flex, gcc, gcc-c++, gettext, libtool, make, patch, pkgconfig, lua,  git, redhat-rpm-config, rpm-build, rpm-sign,  elfutils, patchutils, cmake")
  set(CPACK_PACKAGE_DESCRIPTION "Frate a package/build system manager for C/C++")
  set(CPACK_PACKAGE_CONTACT "Lucas Sproule frate@frate.dev")
endif()
  set(CPACK_GENERATOR "DEB")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Lucas Sproule & Jon-Michael Hartway")
  set(CPACK_PACKAGE_VERSION_MAJOR ${VMAJOR})
  set(CPACK_PACKAGE_VERSION_MINOR ${VMINOR})
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "build-essential, cmake, git,  ccache,  libssh2-1,  lua5.4, liblua5.4-dev")
  set(CPACK_PACKAGE_DESCRIPTION "Frate a package/build system manager for C/C++")
  set(CPACK_PACKAGE_CONTACT "Lucas Sproule frate@frate.dev")

include(CPack)






