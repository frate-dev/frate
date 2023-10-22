#include <iostream>
#include "Command.h"


namespace Command{

  int help() {
    std::cout << "usage: " << ENDL  
      "init: initializes your project" << ENDL
      "run : builds and runs your project" <<ENDL
      "flags: adds a flag to your project" << ENDL;
    return 1;
  }

}
