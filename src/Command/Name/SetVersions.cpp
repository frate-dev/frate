#include <Frate/Interface.hpp>
#include <Frate/Command/Set.hpp>
#include <Frate/Project.hpp>

namespace Frate::Command::Set{

  bool versions(std::shared_ptr<Interface> inter){
    std::string version = inter->args->operator[]("args").as<std::string>();
    inter->pro->version = version;
    return true;
  }
}
