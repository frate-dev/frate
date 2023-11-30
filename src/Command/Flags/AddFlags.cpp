#include "Frate/Command/Flags.hpp"
#include "Frate/Generators.hpp"
namespace Command::Flags {
  bool add(Interface *inter) {
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
          std::cout << "Writing frate-project.json" << std::endl;
          Generators::ConfigJson::writeConfig(inter->pro);
          Generators::CMakeList::createCMakeLists(inter->pro);
          return true;
        }
      }
    }
    for (std::string flag : flags) {
      std::cout << "Adding flag: " << flag << std::endl;
      inter->pro->flags.push_back(flag);
    }
    std::cout << "Writing frate-project.json" << std::endl;
    Generators::ConfigJson::writeConfig(inter->pro);
    return true;
  }
}
