#pragma once
#include <Frate/Package.hpp>
#include "Frate/Dependency.hpp"
namespace Frate::Command {
  class Mode{
    public:
      std::string name;
      std::vector<std::string> flags;
      std::vector<Dependency> dependencies{};
      friend void from_json(const json &j, Mode& mode);
      friend void to_json(json &j, const Mode& mode);
      Mode(
          std::string name = "",
          std::vector<std::string> flags = {},
          std::vector<Dependency> dependencies = {}
          );
  };
}
