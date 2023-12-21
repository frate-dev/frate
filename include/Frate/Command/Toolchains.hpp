#pragma once
#include "../Command.hpp"


namespace Frate::Command {
  namespace Toolchains {
    json load();
    bool list(std::shared_ptr<Interface> inter);
    bool add(std::shared_ptr<Interface> interface);
    bool remove(std::string user_toolchain, std::shared_ptr<Interface> interface);
  }
}
