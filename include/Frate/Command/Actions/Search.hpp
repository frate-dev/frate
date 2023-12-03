#pragma once
#include <Frate/Command.hpp>

namespace Frate::Command::Search {
  bool options(Interface* inter);
  bool run(Interface* inter);
  std::vector<Handler> handlers(Interface* inter);
}
