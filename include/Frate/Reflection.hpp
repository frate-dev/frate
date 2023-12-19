#pragma once
#include <Frate/Command.hpp>
#include <nlohmann/json.hpp>

namespace Frate::Reflection {
  using nlohmann::json;
  using Command::Project;
  //Project related reflection
  // void to_json(const Project &p, json &j);
  // void to_json(const Command::Package &p, json &j);
  // void to_json(const Command::Mode &m, json &j);

  // void from_json(const json &j, Command::Package& package);
  // void from_json(const json &j, Command::Mode& mode);
  // void from_json(const json &j, Project &pro);
}
