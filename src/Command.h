#include <memory>
#include <string>
#include <vector>

namespace Command {

bool handleCppProject();

bool handleCProject();


typedef struct Context {
  std::string project_name;
  std::string git;
  std::string lang;
  std::string langversion;
  std::vector<std::string> authors;
} Context;

void loadPackageToml(std::shared_ptr<Context> ctx);
int init(std::shared_ptr<Context>);
int addLib(std::shared_ptr<Context>, std::string);
}; // namespace Command
