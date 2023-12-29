#pragma once
#include <string>

namespace Frate::System {
  class GitCommit {
  public:
    GitCommit();
    std::string hash;
    std::string author;
    std::string date;
    std::string message;
    friend std::ostream &operator<<(std::ostream &os, const GitCommit &commit);
  };
} // namespace Frate::System
