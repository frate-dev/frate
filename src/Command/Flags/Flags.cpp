#include <CMaker/Command/Flags.hpp>
#include <CMaker/Generators.hpp>

namespace Command::Flags {
  //TODO: This should be moved to the flags module
  bool add(Interface *inter) {
    std::cout << "Adding flags" << std::endl; 
    std::vector<std::string> raw_flags = inter->args->unmatched();
    std::vector<std::string> flags;
    std::string build_flags = "";
    for (std::string flag : raw_flags) {
      if (flag[0] == '-' && flag[1] == '-') {
        build_flags = "-" + build_flags;
        flags.push_back(build_flags);
        build_flags = "";
      }
      flag.erase(std::remove(flag.begin(), flag.end(), '-'), flag.end());
      build_flags += flag;
    }
    if (build_flags != "") {
      build_flags = "-" + build_flags;
      flags.push_back(build_flags);
    }
    if (inter->args->count("mode") > 0) {
      std::string mode = inter->args->operator[]("mode").as<std::string>();
      for (Mode &m : inter->pro->modes) {
        if (m.name == mode) {
          for (std::string flag : flags) {
            std::cout << "Adding flag: " << flag << std::endl;
            m.flags.push_back(flag);
          }
          std::cout << "Writing config.json" << std::endl;
          Generators::ConfigJson::writeConfig(inter->pro);
          Generators::CMakeList::createCMakeListsExecutable(inter->pro);
          return true;
        }
      }
    }
    
    for (std::string flag : flags) {
      std::cout << "Adding flag: " << flag << std::endl;
      inter->pro->flags.push_back(flag);
    }
    std::cout << "Writing config.json" << std::endl;
    Generators::ConfigJson::writeConfig(inter->pro);
    return true;
  }



} 