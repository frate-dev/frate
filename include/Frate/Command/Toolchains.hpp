#pragma once
#include <Frate/Interface.hpp>
#include <nlohmann/json.hpp>

namespace Frate::Command {
  namespace Toolchains {
    using json = nlohmann::json;
    json load();
    bool list(std::shared_ptr<Interface> inter);
    bool add(std::shared_ptr<Interface> interface);
    bool remove(std::string user_toolchain,
                std::shared_ptr<Interface> interface);
  } // namespace Toolchains
} // namespace Frate::Command
