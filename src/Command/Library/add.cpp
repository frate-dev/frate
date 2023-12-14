#include <Frate/Command/Library.hpp>

namespace Frate::Command::Library{
  bool add(std::shared_ptr<Interface> inter){
    options(inter);
    std::vector<std::string> libs = inter->args->operator[]("lib").as<std::vector<std::string>>();
    for (std::string lib : libs){
      inter->pro->libs.push_back(lib);
    }
    return true;
  }
}
