#pragma once
#include <Frate/Command.hpp>

namespace Command::List {

  bool options(Interface* inter);
  bool run(Interface* inter);
  std::vector<Handler> handlers(Interface* inter);
}
