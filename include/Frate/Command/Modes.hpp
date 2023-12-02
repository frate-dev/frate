#pragma once
#include "../Command.hpp"

namespace Frate::Command{
  namespace Modes{
    bool add(Interface* interface);
    bool remove(Interface* interface);
    bool list(Interface* interface);
    bool options(Interface* interface);
  }
}

