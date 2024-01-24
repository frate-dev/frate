#include <Frate/Template/IndexEntry.hpp>
#include <Frate/Utils/Logging.hpp>
#include <Frate/Utils/Macros.hpp>

namespace Frate::Project {
    void from_json(const nlohmann::json &json_obj, TemplateIndexEntry &template_obj){
      FROM_JSON_FIELD(template_obj, name);
      FROM_JSON_FIELD(template_obj, description);
      FROM_JSON_FIELD(template_obj, head);
      FROM_JSON_FIELD(template_obj, git);
      FROM_JSON_FIELD(template_obj, branches);
    }
    void to_json(nlohmann::json &json_obj, const TemplateIndexEntry &template_obj){
      TO_JSON_FIELD(template_obj, name);
      TO_JSON_FIELD(template_obj, description);
      TO_JSON_FIELD(template_obj, head);
      TO_JSON_FIELD(template_obj, git);
      TO_JSON_FIELD(template_obj, branches);
    }
}
