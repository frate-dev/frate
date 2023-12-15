#include <string>
#include <vector>


namespace Frate::Constants {

  #ifdef __linux__
  const std::string BUILD_OS = "linux";
  #elif _WIN32
  const std::string BUILD_OS = "windows32";
  #elif _WIN64
  const std::string BUILD_OS = "windows64";
  #elif __APPLE__
  const std::string BUILD_OS = "macos";
  #else
  const std::string BUILD_OS = "unknown";
  #endif

  const std::string NAME = "frate";
  const std::string DESCRIPTION = "CLI utility for managing your project, a modern touch for C/C++";
  const std::string PROJECT_URL = "https://github.com/frate-dev/frate";

  constexpr const std::string_view FRATE_PACKAGES  = "https://github.com/frate-packages/index/releases/latest/download/index.json";
  constexpr const std::string_view FRATE_TEMPLATES = "https://github.com/frate-templates/templates/releases/latest/download/index.json";
  
  const std::string TEMPLATE_PATH = "template/";
  const std::string INIT_SCRIPTS_PATH = "__init__/";
  const std::string POST_SCRIPTS_PATH = "__post__/";
#ifdef DEBUG
  const std::string TEMPLATE_BRANCH = "dev";
#else
  const std::string TEMPLATE_BRANCH = "main";
#endif
  
  const std::string VERSION = 
    std::to_string(VMAJOR) + "." + 
    std::to_string(VMINOR) + "." + 
    std::to_string(VPATCH)
#ifdef VBUILD
    + "." + std::to_string(VBUILD)
#endif
    ;

  const std::vector<std::string> SUPPORTED_CMAKE_VERSIONS =
  {
    "3.0",
    "3.1", "3.2",
    "3.3",
    "3.4",
    "3.5",
    "3.6",
    "3.7",
    "3.8",
    "3.9",
    "3.10",
    "3.11",
    "3.12",
    "3.13",
    "3.14",
    "3.15",
    "3.16",
    "3.17",
    "3.18",
    "3.19",
    "3.20",
    "3.21",
    "3.22",
    "3.23",
    "3.24",
    "3.25",
    "3.26",
    "3.27",
    "3.28",
  };

  const std::vector<std::string> SUPPORTED_C_COMPILERS =
  {
    "clang",
    "gcc",
    "icc",
    "msvc",
  };

  const std::vector<std::string> SUPPORTED_CXX_COMPILERS =
  {
    "clang++",
    "g++",
    "icpc",
    "msvc",
  };

  const std::vector<std::string> SUPPORTED_C_STANDARDS =
  {
    "89",
    "90",
    "99",
    "11",
    "17",
    "18",
    "2x",
  };
  
  const std::vector<std::string> SUPPORTED_CXX_STANDARDS =
  {
    "98",
    "03",
    "11",
    "1x",
    "14",
    "17",
    "20",
    "2x",
    "23",
  };

  const std::vector<std::string> SUPPORTED_LANGUAGES =
  {
    "c",
    "cpp",
  };


}
