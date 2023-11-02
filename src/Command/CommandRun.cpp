#include <iostream>
#include "Command.hpp"
#include <string>

namespace Command {
  bool Interface::run(){
    #ifdef DEBUG
      int success = system("mkdir -p build/build/");
      if (success != 0) {
        std::cout << "Error creating build directory" << std::endl;
        return false;
      }
      success = system("cd build && cmake  . && make  && cd ..");
      if (success != 0) {
        std::cout << "Error building project" << std::endl;
        return false;
      }
    #else
      system("mkdir -p build");
      system("cmake  . ");
      system("make");
    #endif
    std::string file_name = "./build/";
    #ifdef DEBUG
      file_name = "./build/build/";
    #endif
    std::string command = file_name + ctx->project_name;
    success = system(command.c_str());
    if (success != 0) {
      std::cout << "Error running project" << std::endl;
      return false;
    }
    return true;
  }
}

