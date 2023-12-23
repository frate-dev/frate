#pragma once
#include <Frate/Interface.hpp>

namespace Frate::Command::Library{
  bool options(std::shared_ptr<Interface> inter);
  bool add(std::shared_ptr<Interface> inter);
  bool remove(std::shared_ptr<Interface> inter);
  bool list(std::shared_ptr<Interface> inter);
}
