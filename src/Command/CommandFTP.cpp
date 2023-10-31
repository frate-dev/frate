#include <iostream>
#include "Command.hpp"

namespace Command{
  bool Interface::ftp() {
    bool test = this->LoadPackageToml();
    if(!test) {
      std::cout << "Error: Could not load config.toml" << std::endl;
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
