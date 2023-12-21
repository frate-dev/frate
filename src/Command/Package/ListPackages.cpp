#include "stdint.h"
#include "termcolor/termcolor.hpp"
#include <Frate/Command/Package.hpp>
#include <Frate/Project.hpp>

namespace Frate::Command::Packages {
  bool list(std::shared_ptr<Interface> inter) {
    std::cout << "Listing packages" << std::endl;
    std::cout << "Packages: " << std::endl;
    for (Package &p : inter->pro->dependencies) {
      std::cout << "\t" << p.name << " : " << termcolor::bright_blue << p.selected_version << termcolor::reset << ENDL;
    }
    return true;
  }
}
