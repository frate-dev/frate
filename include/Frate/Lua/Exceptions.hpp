#pragma once
#include "Frate/FrateException.hpp"

namespace Frate::Lua {

  class LuaException : public FrateException {
  public:
    LuaException(const std::string &message) : FrateException(message) {}
  };

} // namespace Frate::Lua
