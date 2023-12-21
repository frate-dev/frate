#include "Frate/System.hpp"
namespace Frate::System {

  Capability::Capability(){
  }
  void from_json(const nlohmann::json &j, Capability& capability);
  void to_json(nlohmann::json &j, const Capability& capability);
}
