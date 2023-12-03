#include <Frate/Command.hpp>
#include <Frate/Command/Set.hpp>

namespace Frate::Command::Set{

  bool versions(Interface* inter){
    std::string version = inter->args->operator[]("args").as<std::string>();
    inter->pro->project_version = version;
    return true;
  }
}
