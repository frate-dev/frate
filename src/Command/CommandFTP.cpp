#include <iostream>
#include "Command.hpp"
#include "../Utils/CLI.hpp"

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
    system("rm -rf ./build/*");
   #else
      system("rm -rf ./*");
    #endif // DEBUG
    return true;
  }
}
