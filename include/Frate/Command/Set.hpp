#pragma once
#include <Frate/Interface.hpp>


namespace Frate::Command::Set{
  bool options(std::shared_ptr<Interface> inter);
  bool set(std::shared_ptr<Interface> inter);
}
