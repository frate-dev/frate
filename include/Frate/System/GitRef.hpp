#pragma once
#include <string>

namespace Frate::System {
  class GitRef {
  public:
    GitRef();
    std::string name;
    std::string hash;
    friend std::ostream &operator<<(std::ostream &os_stream, const GitRef &ref);
  };
} // namespace Frate::System
