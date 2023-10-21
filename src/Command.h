#include <memory>
#include <string>
#include <vector>
#include <filesystem>

#define ENDL "\n"

namespace Command {

bool handleCppProject();

bool handleCProject();


typedef struct Context {
  std::string project_name{"project"};
  std::string git{"https://github.com"};
  std::string lang{"cpp"};
  std::string langversion{"c++17"};
  std::vector<std::string> authors{"author"};
  std::string src_dir{"src"};
  std::string build_dir{"build"};
} Context;

void loadPackageToml(std::shared_ptr<Context> ctx);
int init(std::shared_ptr<Context>);
int addLib(std::shared_ptr<Context>, std::string);
}; // namespace Command
