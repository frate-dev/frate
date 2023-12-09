#pragma once
#include <Frate/Command.hpp>

namespace Frate::Command::Library{
  bool add(Interface* inter, std::string link);
  bool remove(Interface* inter, std::string link);
  bool list(Interface* inter);
}
