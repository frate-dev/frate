#include "Frate/Utils/General.hpp"
#include <Frate/Command/Name.hpp>

namespace Frate::Command::Name {
  bool set(std::shared_ptr<Interface> inter) {
    options(inter);
    if(inter->args->count("name") == 0) {
      Utils::error << "No name given.\n";
      return false;
    }
    inter->pro->name = inter->args->operator[]("name").as<std::string>();
    Utils::info << "Name set to " << inter->pro->name << ".\n";

    inter->pro->save();

    return true;
  }
}
