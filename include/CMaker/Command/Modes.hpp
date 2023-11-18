#pragma once
#include "../Command.hpp"

namespace Command{
  namespace Modes{
    bool add(Interface* interface);
    bool remove(Interface* interface);
    bool list(Interface* interface);
  }
}

