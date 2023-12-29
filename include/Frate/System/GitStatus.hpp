#pragma once
#include <string>
#include <vector>

namespace Frate::System {
  class GitStatus {
  public:
    GitStatus();
    std::vector<std::string> added;
    std::vector<std::string> modified;
    std::vector<std::string> deleted;
    std::vector<std::string> renamed;
    std::vector<std::string> copied;
    std::vector<std::string> untracked;
    std::vector<std::string> ignored;
    friend std::ostream &operator<<(std::ostream &os, const GitStatus &status);
  };
} // namespace Frate::System
