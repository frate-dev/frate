#pragma once
#include <string>
#include <vector>
namespace Frate::Command::Toolchain {
  typedef struct CompileTarget {
    std::string os;
    std::string triple;
    std::string processor{""};
  } CompileTarget;
  extern const std::vector<CompileTarget> CompileTargets;
}
