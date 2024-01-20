#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace Frate::Project {

  class TemplateIndexEntry {
  private:
    std::string name;
    std::string description;
    std::string latest_hash;
    std::string git;

  public:
    friend void from_json(const nlohmann::json &j, TemplateIndexEntry &t);
    friend void to_json(nlohmann::json &j, const TemplateIndexEntry &t);

    std::string &getName() { return name; }

    std::string &getDescription() { return description; }

    std::string &getLatestHash() { return latest_hash; }

    std::string &getGit() { return git; }
  };
} // namespace Frate::Project
