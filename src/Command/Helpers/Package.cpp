#include <Frate/Package.hpp>
#include <Frate/Utils/Macros.hpp>
#include <Frate/Utils/Logging.hpp>

namespace Frate::Command {
  void to_json(json &j, const Package &package) {
    TO_JSON_FIELD(package,name);
    // TO_JSON_FIELD(package,selected_version);
    TO_JSON_FIELD(package,versions);
    TO_JSON_FIELD(package,stars);
    TO_JSON_FIELD(package,forks);
    TO_JSON_FIELD(package,open_issues);
    TO_JSON_FIELD(package,watchers);
    // TO_JSON_FIELD(package,score);
    TO_JSON_FIELD(package,git);
    TO_JSON_FIELD(package,git_short);
    TO_JSON_FIELD(package,git_prefixed);
    TO_JSON_FIELD(package,git_description);
    TO_JSON_FIELD(package,target_link);
    TO_JSON_FIELD(package,description);
    TO_JSON_FIELD(package,language);
    TO_JSON_FIELD(package,license);
    TO_JSON_FIELD(package,owner);
    TO_JSON_FIELD(package,owner_type);
  }
  void from_json(const json &j, Package &package){
    FROM_JSON_FIELD(package,name);
    // FROM_JSON_FIELD(package,selected_version);
    FROM_JSON_FIELD(package,versions);
    FROM_JSON_FIELD(package,stars);
    FROM_JSON_FIELD(package,forks);
    FROM_JSON_FIELD(package,open_issues);
    FROM_JSON_FIELD(package,watchers);
    // FROM_JSON_FIELD(package,score);
    FROM_JSON_FIELD(package,git);
    FROM_JSON_FIELD(package,git_short);
    FROM_JSON_FIELD(package,git_prefixed);
    FROM_JSON_FIELD(package,git_description);
    FROM_JSON_FIELD(package,target_link);
    FROM_JSON_FIELD(package,description);
    FROM_JSON_FIELD(package,language);
    FROM_JSON_FIELD(package,license);
    FROM_JSON_FIELD(package,owner);
    FROM_JSON_FIELD(package,owner_type);
  }
}
