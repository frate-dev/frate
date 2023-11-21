#include <iostream>
#include <string>
#include <cstdbool>
#ifdef __linux__
#include <sys/ioctl.h>
#endif
#include <Frate/Command.hpp>
#include <Frate/Utils/General.hpp>
#include <termcolor/termcolor.hpp>

namespace Command {
  //We're keeping this for our long lost friend ryan aka fired docs guy
  bool oldHelp() {
#ifdef __linux__
    if ((static_cast<std::string>(std::getenv("TERM")) == "xterm-kitty")){
      struct winsize w = {500, 500, 0, 0};
      ioctl(0, TIOCGWINSZ, &w);
      int success = Utils::hSystem("kitty +kitten icat --align left ~/Downloads/logo.png");
      if (success != 0) {
        std::cout << "Error printing logo" << std::endl;
        return false;
      }
    }
#endif

    std::cout << "usage: "<< termcolor::green << "frate"<< termcolor::reset <<" <sub-command>" << ENDL  
      "Commands:" << ENDL
      "\t i | init [<package-name>]" << ENDL
      "\t          [-y | --skip-init]" << ENDL
      "\t          [-n | --name example-name] " << ENDL
      "\t          [-l | --language cpp/c]:" << ENDL
      "\t          initializes your project" << ENDL
      "\t run:   builds and runs your project" << ENDL
      "\t watch:   watches builds and runs your project on changes" << ENDL
      "\t add  [dep, lib, flags]: add library, dependency or flags to your project" << ENDL
      "\t modes [Debug, Test, Release]: Add and Remove compilation modes/definitions" << ENDL
      "\t search <query>: search for a package" << ENDL
      //TODO server is now RemoteBuildServer and will be in add
      "\t server: manages remote build servers for your projects" << ENDL
      "\t ftp:  deletes the entire project (F*ck this project)" << ENDL
      // RYANS LAST CODE BLOCK IN ENTIRE REPO MUST BE SAVED DO NOT TOUCH
      "\t help:  print help"  << ENDL;
      // SACRED CODE BLOCK ENDS HERE
    std::cout << termcolor::red << "This is a pre-alpha version of frate, please report any bugs here: " << termcolor::reset
      << " https://github.com/frate-dev/cmaker/issues" << ENDL;

    return 1;
  }
  bool Interface::help() {
  #ifdef __linux__
    if((static_cast<std::string>(std::getenv("TERM")) == "xterm-kitty")){
      struct winsize w = {500, 500, 0, 0};
      ioctl(0, TIOCGWINSZ, &w);
      int success = Utils::hSystem("kitty +kitten icat --align left ~/Downloads/logo.png");
      if(success != 0){
        std::cout << "Error printing logo" << std::endl;
        return false;
      }
    }
  #endif
    getHelpString("frate",commands);
    return true;
  }
}
