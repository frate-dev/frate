#pragma once
#include <Frate/Command.hpp>

namespace Frate::Command::FTP {

  bool options(std::shared_ptr<Interface> inter);
  bool run(std::shared_ptr<Interface> inter);
  std::vector<Handler> handlers(std::shared_ptr<Interface> inter);
}
