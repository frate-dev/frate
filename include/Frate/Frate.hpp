#pragma once
#include <Frate/Utils/General.hpp>

namespace Frate {
  template <class T> using Some = std::variant<T, bool>;
}
