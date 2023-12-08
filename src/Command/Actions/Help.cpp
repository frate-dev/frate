#include <Frate/Command/Actions/Help.hpp>
#include <Frate/Utils/General.hpp>
#include <cstdbool>
#include <sys/ioctl.h>
namespace Frate::Command::Help {
  //We're keeping this for our long lost friend ryan aka fired docs guy
  bool run(Interface* inter) {
   // if((static_cast<std::string>(std::getenv("TERM")) == "xterm-kitty")){
   //   struct winsize w = {500, 500, 0, 0};
   //   ioctl(0, TIOCGWINSZ, &w);
   //   int success = Utils::hSystem("kitty +kitten icat --align left ~/Downloads/logo.png");
   //   if(success != 0){
   //     std::cout << "Error printing logo" << std::endl;
   //     return false;
   //   }
   // }
    inter->getHelpString("frate",inter->commands);
    return true;
  }
}
