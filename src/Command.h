#include <memory>
#include "toml.hpp"
#include <map>
#include <string>
#include <vector>
#include <filesystem>

#define ENDL "\n"

namespace Command {

bool handleCppProject();

bool handleCProject();



typedef struct Context {
  /*
   * TODO: This should be initialize to the current directory if not specified
   */
  std::string project_name;
  std::string git{"null"};
  std::string lang{"cpp"};
  std::string cmake_version{"3.10"};
  std::string lang_version{"20"};
  std::string compiler{"g++"};
  std::vector<std::string> authors;
  std::string src_dir{"src"};
  std::string include_dir{"include"};
  std::string build_dir{"build"};
  std::string project_version{"0.0.1"};
  std::vector<std::string> flags;
} Context;

int loadPackageToml(std::shared_ptr<Context> ctx);
int init(std::shared_ptr<Context>);
int run(std::shared_ptr<Context>);
int addFlag(std::shared_ptr<Context>, std::string);
int help();

int writePackageToml(std::shared_ptr<Context> ctx);
int addLib(std::shared_ptr<Context>, std::string);
}; // namespace Command
