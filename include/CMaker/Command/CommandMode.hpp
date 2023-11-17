#pragma once
#include "Command.hpp"

namespace Command::ModeCommands {
  bool addFlags(Interface* inter, std::string mode);
  bool removeFlags(Interface* inter, std::string mode);

  bool addPackages(Interface* inter, std::string mode);

  bool removePackages(Interface* inter, std::string mode);
}
