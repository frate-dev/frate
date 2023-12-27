#include <Frate/Command/Library.hpp>
#include <Frate/Project.hpp>

namespace Frate::Command::Library {
  bool remove(std::shared_ptr<Interface> inter) {
    std::vector<std::string> libs =
        inter->args->operator[]("lib").as<std::vector<std::string>>();
    std::erase_if(inter->pro->libs, [&libs](std::string &lib) {
      for (std::string &lib2 : libs) {
        if (lib == lib2) {
          return true;
        }
      }
      return false;
    });
    return true;
  }
} // namespace Frate::Command::Library
