#include <iostream>
#include "Command.hpp"
#include <string>

namespace Command {
  bool Interface::run(){
    std::cout << "Running project: " << pro->project_name << std::endl;
    #ifdef DEBUG
      int success = system("mkdir -p build/build/");
      if (success != 0) {
        std::cout << "Error creating build directory" << std::endl;
        return false;
      }
      success = system("cd build ;cmake . ;make");
      if (success != 0) {
        std::cout << "Error building project" << std::endl;
        return false;
      }
    #else
      int success = system("mkdir -p build");
      if (success != 0) {
        std::cout << "Error creating build directory" << std::endl;
        return false;
      }
      system("cmake  . ");
      if (success != 0) {
        std::cout << "Error building project cmake" << std::endl;
        return false;
      }
      system("make");
      if (success != 0) {
        std::cout << "Error building project make" << std::endl;
        return false;
      }
    #endif
    std::string command = std::string(pro->project_path) + "/build/" + pro->project_name;
    success = system(command.c_str());
    if (success != 0) {
      std::cout << "Error running project!" << std::endl;
      return false;
    }

    if (success != 0) {
      std::cout << "Error running project" << std::endl;
      return false;
    }
    return true;
  }
}

