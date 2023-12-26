#pragma once
#include "Frate/Package.hpp"
#include <string>

namespace Frate::Command {
  class Dependency {
  public:
    Dependency();
    Dependency(Package &package, std::string &version);
    std::string name;
    std::string version;
    std::string git;
    std::string git_short;
    std::string git_prefixed;
    friend void to_json(json &json_obj, const Dependency &dep);
    friend void from_json(const json &json_obj, Dependency &dep);
  };
} // namespace Frate::Command
