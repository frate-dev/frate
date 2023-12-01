#include "Frate/Utils/General.hpp"
#include <Frate/Command/Set.hpp>

namespace Command::Set{
  bool name(Interface* inter) {
    Set::options(inter);
    Utils::Info info;
    Utils::Error error;
    if(inter->args->count("name") == 0) {
      error << "No name given.\n";
      return false;
    }
    inter->pro->project_name = inter->args->operator[]("name").as<std::string>();
    info << "Name set to " << inter->pro->project_name << ".\n";

    inter->pro->save();

    return true;
  }
}
