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
# curl (unversioned)
# CPMDeclarePackage(curl
#  NAME curl
#  GIT_TAG curl-8_3_0
#  GITHUB_REPOSITORY curl/curl
#  OPTIONS
#    "ENABLE_VERSIONED_SYMBOLS ON"
#)
# Catch2
CPMDeclarePackage(Catch2
  VERSION 3.4.0
  GITHUB_REPOSITORY catchorg/Catch2
  SYSTEM YES
  EXCLUDE_FROM_ALL YES
)
# cpptrace
CPMDeclarePackage(cpptrace
  VERSION 0.2.1
  GITHUB_REPOSITORY jeremy-rifkin/cpptrace
  SYSTEM YES
  EXCLUDE_FROM_ALL YES
)
