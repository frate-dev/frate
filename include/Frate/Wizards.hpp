#pragma once
#include "Frate/Project/Config.hpp"
#include <Frate/RemoteServer.hpp>
#include <memory>

namespace Frate::Wizard {
  bool Project(std::shared_ptr<Project::Config> project);
  bool RemoteServer(std::shared_ptr<Command::RemoteServer> server);
} // namespace Frate::Wizard
