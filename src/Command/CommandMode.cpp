#include "Command.hpp"

namespace Command {
  bool Interface::mode(){
    std::cout << "Running mode: " << std::endl;

    return true;
  }
};
