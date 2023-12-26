#pragma once
#include "Frate/Project.hpp"
#include <Frate/RemoteServer.hpp>
#include <memory>

namespace Frate::Wizard {
  bool Project(std::shared_ptr<Command::Project> project);
  bool RemoteServer(std::shared_ptr<Command::RemoteServer> server);
} // namespace Frate::Wizard
