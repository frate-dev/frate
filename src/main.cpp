#include "Command/Command.h"
#include <iostream>
#include <memory>
#include <string>

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
