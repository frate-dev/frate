#include <Frate/Command/Library.hpp>
#include <Frate/Project.hpp>

namespace Frate::Command::Library {
  bool add(std::shared_ptr<Interface> inter) {
    options(inter);
    std::vector<std::string> libs =
        inter->args->operator[]("lib").as<std::vector<std::string>>();
    for (std::string lib : libs) {
      inter->pro->libs.emplace_back(lib);
    }
    return true;
  }
} // namespace Frate::Command::Library
