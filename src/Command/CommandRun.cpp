#include <iostream>
#include "Command.hpp"
#include <string>
#include "../Utils/General.hpp"

namespace Command {
  bool Interface::run(){
    std::cout << "Running project: " << pro->project_name << std::endl;

    const std::string work_dir_cmd = "cd " + pro->project_path.string();
    const std::string mkdir_build_dir = work_dir_cmd + ";mkdir -p build";
    const std::string cmake_cmd = work_dir_cmd + ";cmake .";
    const std::string make_cmd = work_dir_cmd + ";make";
    const std::string run_cmd = work_dir_cmd + ";./build/" + pro->project_name;

    /*
     * Creating build directory and building project
     */
    int success = Utils::hSystem(mkdir_build_dir);

    if (success != 0) {
      std::cout << "Error creating build directory" << std::endl;
      return false;
    }

    success = Utils::hSystem(cmake_cmd);
    if (success != 0) {
      std::cout << "Error running cmake" << std::endl;
      return false;
    }

    success = Utils::hSystem(make_cmd);
    if (success != 0) {
      std::cout << "Error running make" << std::endl;
      return false;
    }

    /*
     * Running project
     */

    success = Utils::hSystem(run_cmd);
    if (success != 0) {
      std::cout << "Error running project" << std::endl;
      return false;
    }

    return true;
  }
}

