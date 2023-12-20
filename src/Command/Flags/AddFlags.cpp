#include "Frate/Command/Flags.hpp"
#include <Frate/Project.hpp>
namespace Frate::Command::Flags {
  bool add(std::shared_ptr<Interface> inter) {
    options(inter);
    std::cout << "Adding flags" << std::endl; 
    std::vector<std::string> flags = makeFlags(inter);

    if (inter->args->count("mode") > 0) {
      std::string mode = inter->args->operator[]("mode").as<std::string>();
      for (Mode &m : inter->pro->modes) {
        if (m.name == mode) {
          for (std::string flag : flags) {
            std::cout << "Adding flag: " << flag << std::endl;
            m.flags.push_back(flag);
          }
          return true;
        }
      }
    }
    for (std::string flag : flags) {
      std::cout << "Adding flag: " << flag << std::endl;
      inter->pro->flags.push_back(flag);
    }
    return true;
  }
}
