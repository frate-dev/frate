#include <iostream>
#include "Command.hpp"
#include "../Utils/CLI.hpp"
#include "../Utils/General.hpp"

namespace Command{
  using Utils::CLI::Prompt;
  using namespace Utils::CLI::Ansi;
  bool Interface::ftp() {

    Prompt<bool> *prompt = new Prompt<bool>("Are you sure you would like to delete the entire project?");
    prompt->Color(RED)->Run();
    if(!prompt->Get()) {
      std::cout << "Aborting..." << std::endl;
      return false;
    }
  #ifdef DEBUG
    if(Utils::hSystem("rm -rf ./build/*") != 0){
      std::cout << "Error deleting build directory" << std::endl;
      return false;
    }
   #else
      if(Utils::hSystem("rm -rf ./*") != 0){
        std::cout << "Error deleting project directory" << std::endl;
        return false;
      }
    #endif // DEBUG
    return true;
  }
}
