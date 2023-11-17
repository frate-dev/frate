#include "Command.hpp"


namespace Command {
  namespace Toolchains {
    json load();
    bool list();
    bool add(std::string toolchain, Interface* interface);
    bool remove(std::string user_toolchain, Interface* interface);
  }
}
