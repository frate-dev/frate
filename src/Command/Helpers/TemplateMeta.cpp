#include <Frate/TemplateMeta.hpp>
#include <Frate/Utils/Logging.hpp>
#include <Frate/Utils/Macros.hpp>

namespace Frate::Command {

  TemplateMeta::TemplateMeta(const nlohmann::json &json_obj) {
    from_json(json_obj, *this);
  }

  void from_json(const nlohmann::json &json_obj, TemplateMeta &temp) {
    FROM_JSON_FIELD(temp, name);
    FROM_JSON_FIELD(temp, description);
    FROM_JSON_FIELD(temp, hash);
    FROM_JSON_FIELD(temp, git);
  }

  void to_json(nlohmann::json &json_obj, const TemplateMeta &temp) {
    TO_JSON_FIELD(temp, name);
    TO_JSON_FIELD(temp, description);
    TO_JSON_FIELD(temp, hash);
    TO_JSON_FIELD(temp, git);
  }

  std::ostream &operator<<(std::ostream &os_stream, const TemplateMeta &temp) {
    os_stream << "Name: " << temp.name << std::endl;
    os_stream << "Description: " << temp.description << std::endl;
    os_stream << "Hash: " << temp.hash << std::endl;
    os_stream << "Git: " << temp.git << std::endl;
    return os_stream;
  }

} // namespace Frate::Command
