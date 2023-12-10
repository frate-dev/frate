#pragma once
#include <Frate/Command.hpp>

namespace Frate::Command::Library{
  bool options(Interface* inter);
  bool add(Interface* inter);
  bool remove(Interface* inter);
  bool list(Interface* inter);
}
