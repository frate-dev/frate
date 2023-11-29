#include <string>
#include <vector>


namespace Constants {
  const std::string NAME = "frate";
  const std::string DESCRIPTION = "A C/C++ project manager";
  const std::string PROJECT_URL = "https://github.com/frate-dev/frate";
  const std::vector<std::string> SUPPORTED_CMAKE_VERSIONS =
  {
    "3.0",
    "3.1",
    "3.2",
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
    "c89",
    "c90",
    "c99",
    "c11",
    "c17",
    "c18",
    "c2x",
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
