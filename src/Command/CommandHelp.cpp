#include <iostream>
#include <string>
#include <cstdbool>
#include <sys/ioctl.h>
#include "Command.hpp"

namespace Command {
  bool help() {
    if ((static_cast<std::string>(std::getenv("TERM")) == "xterm-kitty")){
      struct winsize w = {500, 500, 0, 0};
      ioctl(0, TIOCGWINSZ, &w);
      system("kitty +kitten icat --align left ~/Downloads/logo.png");
    }

    std::cout << "usage: cmaker <sub-command>" << ENDL  
      "Commands:" << ENDL
      "\t i | init [<package-name>]" << ENDL
      "\t          [-y | --skip-init]" << ENDL
      "\t          [-n | --name example-name] " << ENDL
      "\t          [-l | --language cpp/c]:" << ENDL
      "\t          initializes your project" << ENDL
      "\t run:   builds and runs your project" << ENDL
      "\t dev:   watches builds and runs your project on changes" << ENDL
      "\t add  [subcommand dep lib, flags]: add library, dependency or flags to your project" << ENDL
      "\t ftp:  deletes the entire project" << ENDL
      "\t flags: adds a flag to your project"  << ENDL
      "\t help:  print help"  << ENDL;
    return 1;
  }
}
