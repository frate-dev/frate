#include <iostream>
#include "Command.hpp"

namespace Command{
  bool ftp(std::shared_ptr<Context> ctx) {
    bool test = loadPackageToml(ctx);
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
