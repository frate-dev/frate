#include <iostream>
#include <string>
#include <cstdbool>
#include <sys/ioctl.h>
#include <CMaker/Command.hpp>
#include <CMaker/Utils/General.hpp>
#include <termcolor/termcolor.hpp>

namespace Command {
  //We're keeping this for our long lost friend ryan aka fired docs guy
  bool oldHelp() {
    if ((static_cast<std::string>(std::getenv("TERM")) == "xterm-kitty")){
      struct winsize w = {500, 500, 0, 0};
      ioctl(0, TIOCGWINSZ, &w);
      int success = Utils::hSystem("kitty +kitten icat --align left ~/Downloads/logo.png");
      if (success != 0) {
        std::cout << "Error printing logo" << std::endl;
        return false;
      }
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
      "\t modes [Debug, Test, Release]: Add and Remove compilation modes/definitions" << ENDL
      "\t search <query>: search for a package" << ENDL
      //TODO server is now RemoteBuildServer and will be in add
      "\t server: manages remote build servers for your projects" << ENDL
      "\t ftp:  deletes the entire project (F*ck this project)" << ENDL
      // RYANS LAST CODE BLOCK IN ENTIRE REPO MUST BE SAVED DO NOT TOUCH
      "\t help:  print help"  << ENDL;
      // SACRED CODE BLOCK ENDS HERE
    std::cout << termcolor::red << "This is a pre-alpha version of cmaker, please report any bugs here: " << termcolor::reset
      << " https://github.com/cmaker-dev/cmaker/issues" << ENDL;

    return 1;
  }
  bool Interface::help() {
    if((static_cast<std::string>(std::getenv("TERM")) == "xterm-kitty")){
      struct winsize w = {500, 500, 0, 0};
      ioctl(0, TIOCGWINSZ, &w);
      int success = Utils::hSystem("kitty +kitten icat --align left ~/Downloads/logo.png");
      if(success != 0){
        std::cout << "Error printing logo" << std::endl;
        return false;
      }
    }

    std::cout << R"(
usage: cmaker <sub-command>
Commands:
n | new [<directory>]
          [-y | --skip-init]
          [-n | --name example-name] 
          [-l | --language cpp/c]
          : initializes your project

run : builds and runs your project
watch : watches builds and runs your project on changes
clean     [-c | --cache] : cleans the cache
          : cleans the build directory

add : [dep, lib, flags, target] 
          [-l | --latest] : latest version of the package
          : add library, dependency or flags to your project
          : add target to cross compile for your project

remove  [dep, lib, flags, target] 
          [-l | --latest] : latest version of the package
          : remove library, dependency or flags to your project
          : remove target to cross compile for your project

modes [add, remove, list ] : Add and Remove compilation modes/definitions [defaults: Debug, Test, Release]
search <query> : search for a package
server : manages remote build servers for your projects
update [index] : updates the index of packages
ftp : deletes the entire project (F*ck this project)
help : print help
)" << termcolor::red << "This is a pre-alpha version of cmaker, please report any bugs here: " << termcolor::reset;
    return true;
  }
}
