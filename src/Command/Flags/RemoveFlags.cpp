#include <Frate/Command/Flags.hpp>
#include <Frate/Generators.hpp>


namespace Frate::Command::Flags {
  bool remove(Interface *inter){
    std::cout << "Removing flags" << std::endl; 
    std::vector<std::string> flags = makeFlags(inter);
    if (inter->args->count("mode") > 0) {
      std::string mode = inter->args->operator[]("mode").as<std::string>();
      for (Mode &m : inter->pro->modes) {
        if (m.name == mode) {
          for (std::string flag : flags) {
            std::cout << "Removing flag: " << flag << std::endl;
            std::erase_if(m.flags, [&flag](auto &f) {
                return f == flag;
            });
          }
          std::cout << "Writing frate-project.json" << std::endl;
          Generators::ConfigJson::writeConfig(inter->pro);
          Generators::CMakeList::createCMakeLists(inter->pro);
          return true;
        }
      }
    }
    for (std::string flag : flags) {
      std::cout << "Removing flag: " << flag << std::endl;
      std::erase_if(inter->pro->flags, [&flag](auto &f) {
          return f == flag;
      });
    }
    std::cout << "Writing frate-project.json" << std::endl;
    Generators::ConfigJson::writeConfig(inter->pro);
    return true;
  }
}
