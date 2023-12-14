#pragma once
#include <Frate/Command.hpp>


namespace Frate::Command::Name{
  bool options(std::shared_ptr<Interface> inter);
  bool set(std::shared_ptr<Interface> inter);
}
