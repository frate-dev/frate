#include "Frate/System/Capability.hpp"
#include <Frate/Utils/Logging.hpp>
#include <Frate/Utils/Macros.hpp>
namespace Frate::System {

  Capability::Capability() = default;
  void from_json(const nlohmann::json &json_obj, Capability& capability){
    FROM_JSON_FIELD(capability, executable);
    FROM_JSON_FIELD(capability, version);
    FROM_JSON_FIELD(capability, path);
    FROM_JSON_FIELD(capability, installed);
  }
  void to_json(nlohmann::json &json_obj, const Capability& capability){
    TO_JSON_FIELD(capability, executable);
    TO_JSON_FIELD(capability, version);
    TO_JSON_FIELD(capability, path);
    TO_JSON_FIELD(capability, installed);
  }
}
