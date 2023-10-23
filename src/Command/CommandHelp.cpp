#include <iostream>
#include "Command.hpp"

namespace Command {

  bool help() {
    std::cout << "usage: idk do what you want" << ENDL  
      "Commands:" << ENDL
      "\t i | init:  initializes your project"     << ENDL
      "\t r | run:   builds and runs your project" << ENDL
      "\t flags: adds a flag to your project"  << ENDL
      "\t help:  print help"  << ENDL;
    return 1;
  }
}
