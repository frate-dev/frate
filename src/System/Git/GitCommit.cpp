#include <Frate/System/GitCommit.hpp>
#include <Frate/Utils/Logging.hpp>
#include <Frate/Utils/Macros.hpp>
#include <ostream>

namespace Frate::System {
  GitCommit::GitCommit() = default;

  std::ostream &operator<<(std::ostream &os_stream, const GitCommit &commit) {
    os_stream << "Hash: " << commit.hash << std::endl;
    os_stream << "Author: " << commit.author << std::endl;
    os_stream << "Date: " << commit.date << std::endl;
    os_stream << "Message: " << commit.message << std::endl;
    return os_stream;
  }

  void to_json(nlohmann::json &json_obj, const GitCommit &commit) {
    TO_JSON_FIELD(commit, hash);
    TO_JSON_FIELD(commit, author);
    TO_JSON_FIELD(commit, date);
    TO_JSON_FIELD(commit, message);
  }

  void from_json(const nlohmann::json &json_obj, GitCommit &commit) {
    FROM_JSON_FIELD(commit, hash);
    FROM_JSON_FIELD(commit, author);
    FROM_JSON_FIELD(commit, date);
    FROM_JSON_FIELD(commit, message);
  }
} // namespace Frate::System
