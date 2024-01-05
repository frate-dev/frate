#pragma once
#include <Frate/Interface.hpp>

namespace Frate::Command::ModeCommands {
  bool addFlags(std::shared_ptr<Interface> inter, std::string mode);
  [[deprecated("using add packages function in PackageAdd.cpp")]] bool
  addPackages(std::shared_ptr<Interface> inter, std::string mode);
  [[deprecated("using remove packages function in PackageRemove.cpp")]] bool
  removePackages(std::shared_ptr<Interface> inter, std::string mode);
} // namespace Frate::Command::ModeCommands
