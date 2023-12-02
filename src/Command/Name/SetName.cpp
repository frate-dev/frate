#include "Frate/Utils/General.hpp"
#include <Frate/Command/Name.hpp>

namespace Frate::Command::Name {
  bool set(Interface* inter) {
    options(inter);
    if(inter->args->count("name") == 0) {
      Frate::error << "No name given.\n";
      return false;
    }
    inter->pro->project_name = inter->args->operator[]("name").as<std::string>();
    Frate::info << "Name set to " << inter->pro->project_name << ".\n";

    inter->pro->save();

    return true;
  }
}
