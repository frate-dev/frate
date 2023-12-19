#include <Frate/Command.hpp>


namespace Frate::Command {

  Mode::Mode(std::string name, std::vector<std::string> flags, std::vector<Package> dependencies) {
    this->name = name;
    this->flags = flags;
    this->dependencies = dependencies;
  }

  void from_json(const json &j, Command::Mode& mode){
    j.at("name").get_to(mode.name);
    j.at("flags").get_to(mode.flags);
    for (const Command::Package &element : j["dependencies"]) {
      mode.dependencies.push_back(element);
    }
  }
  void to_json(json &j, const Command::Mode &mode){
    j = json{
      {"name", mode.name},
      {"flags", mode.flags},
      {"dependencies", mode.dependencies}
    };
  }
}
