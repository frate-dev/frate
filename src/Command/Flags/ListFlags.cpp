#include <Frate/Command/Flags.hpp>
#include <Frate/Command.hpp>
#include <Frate/Project.hpp>
namespace Frate::Command::Flags {

  bool list(std::shared_ptr<Interface> inter) {
    if (inter->args->count("mode") > 0) {
      std::string mode = inter->args->operator[]("mode").as<std::string>();
      std::cout << "Flags for mode: " << mode << std::endl;
      for (Mode &m : inter->pro->modes) {
        if (m.name == mode) {
          for (std::string flag : m.flags) {
            std::cout << flag << std::endl;
          }
          return true;
        }
      }
    }
    for (std::string flag : inter->pro->flags) {
      std::cout << flag << std::endl;
    }
    return true;
  }
}
 
