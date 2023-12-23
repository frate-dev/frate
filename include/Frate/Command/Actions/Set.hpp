#pragma once
#include <Frate/Interface.hpp>

namespace Frate::Command::Set {


  bool options(std::shared_ptr<Interface> inter);
  bool name(std::shared_ptr<Interface> inter);
  bool run(std::shared_ptr<Interface> inter);
  std::vector<Handler> handlers(std::shared_ptr<Interface> inter);
}
