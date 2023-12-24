#include <Frate/Command/Package.hpp>
#include <Frate/Project.hpp>
#include <Frate/Interface.hpp>
#include "Frate/Dependency.hpp"

namespace Frate::Command::Packages {
  bool list(std::shared_ptr<Interface> inter) {
    std::cout << "Listing packages" << std::endl;
    std::cout << "Packages: " << std::endl;
    for (Dependency &p : inter->pro->dependencies) {
      std::cout << "\t" << p.name << " : " << termcolor::bright_blue << p.version << termcolor::reset << std::endl;
    }
    return true;
  }
}
