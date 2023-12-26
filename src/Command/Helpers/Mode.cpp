#include "Frate/Dependency.hpp"
#include <Frate/Mode.hpp>
#include <Frate/Utils/Logging.hpp>
#include <Frate/Utils/Macros.hpp>

namespace Frate::Command {

  Mode::Mode(std::string name,
             std::vector<std::string> flags,
             std::vector<Dependency> dependencies) {
    this->name = name;
    this->flags = flags;
    this->dependencies = dependencies;
  }

  void from_json(const json &json_obj, Command::Mode &mode) {
    FROM_JSON_FIELD(mode, name);
    FROM_JSON_FIELD(mode, flags);
    FROM_JSON_FIELD(mode, dependencies);
    // j.at("name").get_to(mode.name);
    // j.at("flags").get_to(mode.flags);
    // for (const Command::Package &element : j["dependencies"]) {
    //   mode.dependencies.emplace_back(element);
    // }
  }

  void to_json(json &json_obj, const Command::Mode &mode) {
    TO_JSON_FIELD(mode, name);
    TO_JSON_FIELD(mode, flags);
    TO_JSON_FIELD(mode, dependencies);
    // j = json{
    //   {"name", mode.name},
    //   {"flags", mode.flags},
    //   {"dependencies", mode.dependencies}
    // };
  }
} // namespace Frate::Command
