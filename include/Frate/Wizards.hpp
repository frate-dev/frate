#pragma once
#include <Frate/RemoteServer.hpp>
#include <memory>
#include "Frate/Project.hpp"

namespace Frate::Wizard {
  bool Project(std::shared_ptr<Command::Project> project);
  bool RemoteServer(std::shared_ptr<Command::RemoteServer> server);
}
