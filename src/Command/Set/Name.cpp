#include <Frate/Command.hpp>
#include <Frate/Command/Set.hpp>
#include <Frate/Generators.hpp>

namespace Command::Set{
  bool name(Interface *inter){
    if (inter->pro->args->count("args") != 0) {
      inter->pro->project_name= inter->pro->args->operator[]("args").as<std::string>();
    }
    Generators::ConfigJson::writeConfig(inter->pro);
    Generators::CMakeList::createCMakeLists(inter->pro);

    return true;
  }
}
