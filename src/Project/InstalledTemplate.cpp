#include "Frate/System/GitCommit.hpp"
#include "Frate/Utils/Macros.hpp"
#include <Frate/Project/InstalledTemplate.hpp>
#include <Frate/Utils/Logging.hpp>

namespace Frate::Project {
  System::GitCommit &InstalledTemplate::getLatestCommit() {
    if (&latest == nullptr) {
      // TODO: Iterrate through commits and find the latest
    }
    return latest;
  }

  void to_json(nlohmann::json &json_obj,
               const InstalledTemplate &template_obj) {
    TO_JSON_FIELD(template_obj, name);
    TO_JSON_FIELD(template_obj, commits);
    TO_JSON_FIELD(template_obj, git);
  }

  void from_json(const nlohmann::json &json_obj,
                 InstalledTemplate &template_obj) {
    FROM_JSON_FIELD(template_obj, name);
    FROM_JSON_FIELD(template_obj, commits);
    FROM_JSON_FIELD(template_obj, git);
  }

  std::ostream &operator<<(std::ostream &os_stream,
                           const InstalledTemplate &template_obj) {
    os_stream << "Name: " << template_obj.name << std::endl;
    os_stream << "Commits: " << std::endl;
    for (System::GitCommit commit : template_obj.commits) {
      os_stream << commit << std::endl;
    }
    return os_stream;
  }
} // namespace Frate::Project
