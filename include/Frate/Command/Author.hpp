#pragma once
#include "../Command.hpp"


namespace Command::Author {
  bool add(Interface *inter);
  bool list(Interface *inter);
  bool options(Interface *inter);
  bool remove(Interface *inter);
}
