#include <iostream>
#include <string>
#include <memory>
#include "Command.h"

enum Language { CPP, C };

typedef struct version {
  std::string type;
  std::string version;
} version;

namespace CVersion {
version c89 = {"CMAKE_C_STANDARD_REQUIRED", "89"};
version c90 = {"CMAKE_C_STANDARD_REQUIRED", "90"};
version c99 = {"CMAKE_C_STANDARD_REQUIRED", "99"};
version c11 = {"CMAKE_C_STANDARD_REQUIRED", "11"};
version c17 = {"CMAKE_C_STANDARD_REQUIRED", "17"};
version c18 = {"CMAKE_C_STANDARD_REQUIRED", "18"};
version c2x = {"CMAKE_C_STANDARD_REQUIRED", "2x"};
}; // namespace CVersion

enum ProjectType { BASIC, LIBRARY, EXECUTABLE };




void help() {
  std::cout << "usage: " << "\n";
}

int main(int argc, char **argv) {
  if (argc < 2) {
    help();
    std::cout << "needs more args" << "\n";
    return 1;
  }
  std::string command = argv[1];
  std::shared_ptr<Command::Context> ctx;

  Command::loadPackageToml();
  if (command == "init"){
    Command::init(ctx);
  }
  return 0;
}
