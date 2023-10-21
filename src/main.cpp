#include "Command.h"
#include <iostream>
#include <memory>
#include <string>

enum Language { CPP, C };

typedef struct version {
  std::string type;
  std::string version;
} version;

namespace CVersion {
  version c89 = {"CMAKE_C_STANDARD", "89"};
  version c90 = {"CMAKE_C_STANDARD", "90"};
  version c99 = {"CMAKE_C_STANDARD", "99"};
  version c11 = {"CMAKE_C_STANDARD", "11"};
  version c17 = {"CMAKE_C_STANDARD", "17"};
  version c18 = {"CMAKE_C_STANDARD", "18"};
  version c2x = {"CMAKE_C_STANDARD", "2x"};
}; // namespace CVersion

enum ProjectType { 
  LIBRARY,
  EXECUTABLE 
};

void generateBasicProject(std::string project_name) {}


int main(int argc, char **argv) {
  if (argc < 2) {
    Command::help();
    std::cout << "needs more args"
              << "\n";
    return 1;
  }
  std::string command = argv[1];
  //LUCAS MAKE SURE YOU INITIALIZE YOUR FUCKING STRUCT YOU TWAT
  std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
  if (command == "init") {
    Command::init(ctx);
  }
  if (command == "run") {
    Command::loadPackageToml(ctx);
    Command::run(ctx);
  }
  if (command == "flags"){
    Command::loadPackageToml(ctx);
    Command::addFlag(ctx, argv[2]);
  }
  if (command == "help"){
    Command::help();
  }
  return 0;
}
