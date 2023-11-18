#include "stdint.h"
#include "termcolor/termcolor.hpp"
#include <CMaker/Command/Package.hpp>


namespace Command::Packages {
  bool list(Interface *inter) {
    std::cout << "Listing packages" << std::endl;
    std::cout << "Packages: " << std::endl;
    for (Package &p : inter->pro->dependencies) {
      std::cout << "\t" << p.name << " : " << termcolor::bright_blue << p.selected_version << termcolor::reset << ENDL;
    }
    return true;
  }
}
