#include <iostream>
#include "Command.h"

namespace Command {

  int help() {
    std::cout << "usage: idk do what you want" << ENDL  
      "Commands:" << ENDL
      "\tinit:  initializes your project"     << ENDL
      "\trun:   builds and runs your project" << ENDL
      "\tflags: adds a flag to your project"  << ENDL
      "\thelp:  print help"  << ENDL;
    return 1;
  }

}
