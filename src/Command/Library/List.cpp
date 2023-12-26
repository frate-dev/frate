#include <Frate/Command/Library.hpp>
#include <Frate/Project.hpp>

namespace Frate::Command::Library {
  bool list(std::shared_ptr<Interface> inter) {
    for (std::string &lib : inter->pro->libs) {
      std::cout << lib << std::endl;
    }
    return true;
  }
} // namespace Frate::Command::Library
