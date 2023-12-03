#pragma once
#include <Frate/Command.hpp>

namespace Frate::Command::Watch {

  bool options(Interface* inter);
  bool run(Interface* inter);
  std::vector<Handler> handlers(Interface* inter);
}
namespace Frate::Command::UvWatch {
  bool watch(Interface* inter);
}
