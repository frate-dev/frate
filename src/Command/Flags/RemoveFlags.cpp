#include <Frate/Command/Flags.hpp>
#include <Frate/Generators.hpp>
#include <Frate/Project/Config.hpp>

namespace Frate::Command::Flags {
  bool remove(std::shared_ptr<Interface> inter) {
    options(inter);
    std::cout << "Removing flags" << std::endl;
    std::vector<std::string> flags = makeFlags(inter);
    if (inter->args->count("mode") > 0) {
      std::string mode = inter->args->operator[]("mode").as<std::string>();
      for (Project::Mode &m : inter->pro->modes) {
        if (m.name == mode) {
          for (std::string flag : flags) {
            std::cout << "Removing flag: " << flag << std::endl;
            std::erase_if(m.flags, [&flag](auto &f) { return f == flag; });
          }
          return true;
        }
      }
    }
    for (std::string flag : flags) {
      std::cout << "Removing flag: " << flag << std::endl;
      std::erase_if(inter->pro->flags, [&flag](auto &f) { return f == flag; });
    }
    return true;
  }
} // namespace Frate::Command::Flags
