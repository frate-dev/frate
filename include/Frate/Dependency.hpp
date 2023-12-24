#pragma once
#include <string>
#include "Frate/Package.hpp"
namespace Frate::Command {
  class Dependency {
    public:
      Dependency();
      Dependency(Package &p, std::string &version);
      std::string name;
      std::string version;
      std::string git;
      std::string git_short;
      std::string git_prefixed;
      friend void to_json(json &j, const Dependency &dep);
      friend void from_json(const json &j, Dependency &dep);
  };
}
