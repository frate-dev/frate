#pragma once
#include <Frate/Interface.hpp>

namespace Frate::Command::Help {

  bool options(std::shared_ptr<Interface> inter);
  bool run(std::shared_ptr<Interface> inter);
  std::vector<Handler> handlers(std::shared_ptr<Interface> inter);
} // namespace Frate::Command::Help
