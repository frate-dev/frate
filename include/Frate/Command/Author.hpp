#pragma once
#include <Frate/Interface.hpp>

namespace Frate::Command::Author {
  bool add(std::shared_ptr<Interface> inter);
  bool list(std::shared_ptr<Interface> inter);
  bool options(std::shared_ptr<Interface> inter);
  bool remove(std::shared_ptr<Interface> inter);
}
