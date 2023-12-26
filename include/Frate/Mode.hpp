#pragma once
#include "Frate/Dependency.hpp"
#include <Frate/Package.hpp>

namespace Frate::Command {
  class Mode {
  public:
    std::string name;
    std::vector<std::string> flags;
    std::vector<Dependency> dependencies{};
    friend void from_json(const json &json_obj, Mode &mode);
    friend void to_json(json &json_obj, const Mode &mode);
    Mode(std::string name = "",
         std::vector<std::string> flags = {},
         std::vector<Dependency> dependencies = {});
  };
} // namespace Frate::Command
