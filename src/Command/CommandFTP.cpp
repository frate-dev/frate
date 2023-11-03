#include <iostream>
#include "Command.hpp"

namespace Command{
  bool Interface::ftp() {
    bool test = this->LoadPackageJson();
    if(!test) {
      std::cout << "Error: Could not load config.json" << std::endl;
      return false;
    }
  #ifdef DEBUG
    system("rm -rf ./build/*");
   #else
      system("rm -rf ./*");
    #endif // DEBUG
    return true;
  }
}
