#include <iostream>
#include "Command.hpp"

namespace Command {
  bool help() {
    std::cout << "usage: cmaker <sub-command>" << ENDL  
      "Commands:" << ENDL
      "\t i | init [<package-name>]" << ENDL
      "\t          [-y | --skip-init]" << ENDL
      "\t          [-n | --name example-name] " << ENDL
      "\t          [-l | --language cpp/c]:" << ENDL
      "\t          initializes your project" << ENDL
      "\t r | run:   builds and runs your project" << ENDL
      "\t add <name> <github-url> <branch-tag> : adds a dependency to your project" << ENDL
      "\t ftp:  deletes the entire project" << ENDL
      "\t flags: adds a flag to your project"  << ENDL
      "\t help:  print help"  << ENDL;
    return 1;
  }
}
