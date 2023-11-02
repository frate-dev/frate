#include <iostream>
#include <string>
#include <cstdbool>
#include <sys/ioctl.h>
#include "Command.hpp"
#include <termcolor/termcolor.hpp>

namespace Command {
  bool Interface::help() {
    if ((static_cast<std::string>(std::getenv("TERM")) == "xterm-kitty")){
      struct winsize w = {500, 500, 0, 0};
      ioctl(0, TIOCGWINSZ, &w);
      system("kitty +kitten icat --align left ~/Downloads/logo.png");
    }

    std::cout << "usage: "<< termcolor::green << "cmaker"<< termcolor::reset <<" <sub-command>" << ENDL  
      "Commands:" << ENDL
      "\t i | init [<package-name>]" << ENDL
      "\t          [-y | --skip-init]" << ENDL
      "\t          [-n | --name example-name] " << ENDL
      "\t          [-l | --language cpp/c]:" << ENDL
      "\t          initializes your project" << ENDL
      "\t run:   builds and runs your project" << ENDL
      "\t watch:   watches builds and runs your project on changes" << ENDL
      "\t add  [dep, lib, flags]: add library, dependency or flags to your project" << ENDL
      "\t ftp:  deletes the entire project (F*ck this project)" << ENDL
      "\t flags: adds a flag to your project"  << ENDL
      "\t help:  print help"  << ENDL;
    std::cout << termcolor::red << "This is a pre-alpha version of cmaker, please report any bugs here: " << termcolor::reset
      << " https://github.com/cmaker-dev/cmaker/issues" << ENDL;
    return 1;
  }
}
