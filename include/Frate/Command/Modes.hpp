#pragma once
#include "../Command.hpp"

namespace Command{
  namespace Modes{
    bool add(Interface* inter);
    bool remove(Interface* inter);
    bool list(Interface* inter);
  }
}

