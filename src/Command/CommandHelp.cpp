#include <iostream>
#include "Command.h"

namespace Command {

  int help() {
    std::cout << "usage: idk do what you want" << ENDL  
      "Commands:" << ENDL
      "\t i | init:  initializes your project"     << ENDL
      "\t r | run:   builds and runs your project" << ENDL
      "\tflags: adds a flag to your project"  << ENDL
      "\thelp:  print help"  << ENDL;
    return 1;
  }

}
