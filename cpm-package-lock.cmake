# CPM Package Lock
# This file should be committed to version control

# Ccache.cmake
CPMDeclarePackage(Ccache.cmake
  NAME Ccache.cmake
  VERSION 1.2
  GITHUB_REPOSITORY TheLartians/Ccache.cmake
)
# termcolor
CPMDeclarePackage(termcolor
  NAME termcolor
  GIT_TAG v2.1.0
  GITHUB_REPOSITORY ikalnytskyi/termcolor
)
# cxxopts (unversioned)
# CPMDeclarePackage(cxxopts
#  NAME cxxopts
#  GIT_TAG origin/master
#  GITHUB_REPOSITORY jarro2783/cxxopts
#)
# nlohmann_json (unversioned)
# CPMDeclarePackage(nlohmann_json
#  NAME nlohmann_json
#  GIT_TAG origin/master
#  GITHUB_REPOSITORY nlohmann/json
#)
# libgit2
CPMDeclarePackage(libgit2
  GIT_TAG v1.7.1
  GITHUB_REPOSITORY libgit2/libgit2
  OPTIONS
    "BUILD_TESTS OFF"
    "BUILD_CLI OFF"
  name libgit2
)
# curl (unversioned)
# CPMDeclarePackage(curl
#  NAME curl
#  GIT_TAG curl-8_3_0
#  GITHUB_REPOSITORY curl/curl
#  OPTIONS
#    "ENABLE_VERSIONED_SYMBOLS ON"
#)
# cpptrace
CPMDeclarePackage(cpptrace
  VERSION 0.2.1
  GITHUB_REPOSITORY jeremy-rifkin/cpptrace
  SYSTEM YES
  EXCLUDE_FROM_ALL YES
)
