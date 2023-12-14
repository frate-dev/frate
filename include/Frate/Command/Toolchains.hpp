#include "../Command.hpp"


namespace Frate::Command {
  namespace Toolchains {
    json load();
    bool list();
    bool add(std::string toolchain, std::shared_ptr<Interface> interface);
    bool remove(std::string user_toolchain, std::shared_ptr<Interface> interface);
  }
}
