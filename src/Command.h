#include <memory>
#include <string>
#include <vector>
#include <filesystem>

#define ENDL "\n"

namespace Command {

bool handleCppProject();

bool handleCProject();



typedef struct Context {
  std::string config;
  std::string project_name;
  std::string git;
  std::string lang;
  std::string cmake_version;
  std::string langversion;
  std::string compiler;
  std::vector<std::string> authors;
  std::string src_dir;
  std::string include_dir;
  std::string build_dir;
  std::string semver;
} Context;

void loadPackageToml(std::shared_ptr<Context> ctx);
int init(std::shared_ptr<Context>);
int run(std::shared_ptr<Context>);
int help();
int addLib(std::shared_ptr<Context>, std::string);
}; // namespace Command
