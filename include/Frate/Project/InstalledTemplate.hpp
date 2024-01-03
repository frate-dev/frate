#include "Frate/System/GitCommit.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace Frate::Project {
  class InstalledTemplate {
  private:
    // This will be the latest commit hash, if getLatestCommit() is called more
    // than once, it will return the cached value
    System::GitCommit latest;

  public:
    InstalledTemplate() = default;
    std::string name;
    // Git url to template
    std::string git;
    // The latest commit hash
    std::vector<System::GitCommit> commits;

    // Returns the latest commit hash
    System::GitCommit &getLatestCommit();
    friend void to_json(nlohmann::json &j, const InstalledTemplate &t);
    friend void from_json(const nlohmann::json &j, InstalledTemplate &t);
  };
} // namespace Frate::Project
