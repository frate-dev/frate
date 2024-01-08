#include "Frate/Command/Flags.hpp"
#include <Frate/Project/Config.hpp>

namespace Frate::Command::Flags {
  bool add(std::shared_ptr<Interface> inter) {
    options(inter);
    std::cout << "Adding flags" << std::endl;
    std::vector<std::string> flags = makeFlags(inter);

    if (inter->args->count("mode") > 0) {
      std::string mode = inter->args->operator[]("mode").as<std::string>();
      for (Project::Mode &m : inter->pro->modes) {
        if (m.name == mode) {
          for (std::string flag : flags) {
            std::cout << "Adding flag: " << flag << std::endl;
            m.flags.emplace_back(flag);
          }
          return true;
        }
      }
    }
    for (std::string flag : flags) {
      std::cout << "Adding flag: " << flag << std::endl;
      inter->pro->flags.emplace_back(flag);
    }
    return true;
  }
} // namespace Frate::Command::Flags
