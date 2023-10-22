#include <iostream>
#include "Command.h"
#include <string>

namespace Command {
  bool run(std::shared_ptr<Context> ctx){
    system("mkdir -p build");
    system("cmake  . ");
    system("make");
    std::string command = "./build/" + ctx->project_name;
    system(command.c_str());
    return true;
  }
}
