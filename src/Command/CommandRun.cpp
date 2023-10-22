#include <iostream>
#include "Command.h"
#include <string>

namespace Command {
  bool run(std::shared_ptr<Context> ctx){
    #ifdef DEBUG
      system("cd build");
      system("mkdir -p build/build");
      system("cmake  . ");
      system("make");
      system("cd ..");
    #else
      system("mkdir -p build");
      system("cmake  . ");
      system("make");
    #endif
    std::string file_name = "./build";
    #ifdef DEBUG
      file_name = "./build/build";
    #endif
    std::string command = file_name + ctx->project_name;
    system(command.c_str());
    return true;
  }
}

