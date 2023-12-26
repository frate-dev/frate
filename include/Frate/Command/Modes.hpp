#pragma once
#include <Frate/Interface.hpp>

namespace Frate::Command {
  namespace Modes {
    bool add(std::shared_ptr<Interface> interface);
    bool remove(std::shared_ptr<Interface> interface);
    bool list(std::shared_ptr<Interface> interface);
    bool options(std::shared_ptr<Interface> interface);
  } // namespace Modes
} // namespace Frate::Command
