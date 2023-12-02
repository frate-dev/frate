#pragma once
#include "../Command.hpp"

namespace Frate::Command::ModeCommands {
  bool addFlags(Interface* inter, std::string mode);
  bool removeFlags(Interface* inter, std::string mode);
  [[deprecated("using add packages function in PackageAdd.cpp")]]
  bool addPackages(Interface* inter, std::string mode);
  [[deprecated("using remove packages function in PackageRemove.cpp")]]
  bool removePackages(Interface* inter, std::string mode);
}
