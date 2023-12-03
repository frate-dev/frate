#pragma once
#include <Frate/Command.hpp>

namespace Frate::Command::Set {


  bool options(Interface* inter);
  bool name(Interface *inter);
  bool run(Interface* inter);
  std::vector<Handler> handlers(Interface* inter);
}
