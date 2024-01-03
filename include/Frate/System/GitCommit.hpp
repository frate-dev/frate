#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace Frate::System {
  class GitCommit {
  public:
    GitCommit();
    std::string hash;
    std::string author;
    std::string date;
    std::string message;
    friend std::ostream &operator<<(std::ostream &os, const GitCommit &commit);
    friend void from_json(const nlohmann::json &json_obj, GitCommit &commit);
    friend void to_json(nlohmann::json &json_obj, const GitCommit &commit);
  };
} // namespace Frate::System
