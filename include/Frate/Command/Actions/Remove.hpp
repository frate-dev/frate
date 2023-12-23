#pragma once
#include <Frate/Command.hpp>
#include <Frate/Interface.hpp>

namespace Frate::Command::Remove {

  bool options(std::shared_ptr<Interface> inter);
  bool run(std::shared_ptr<Interface> inter);
  std::vector<Handler> handlers(std::shared_ptr<Interface> inter);
}
