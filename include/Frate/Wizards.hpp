#include "Frate/Utils/CLI.hpp"
#include "Frate/Command.hpp"

namespace Frate::Wizard {
  bool Project(std::shared_ptr<Command::Project> project);
  bool RemoteServer(std::shared_ptr<Command::RemoteServer> server);
}
