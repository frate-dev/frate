#ifdef TEST
#include "Frate/Test/Test.hpp"
#include "nlohmann/json_fwd.hpp"
#include <Frate/Project/Config.hpp>

bool Tests::Command::testAddMode(std::string mode_name) {
  Frate::Utils::info << "Testing add mode command normal conditions"
                     << std::endl;
  if (!testNew()) {
    return false;
  }

  auto [failed, inter] = init("frate add mode " + mode_name);

  if (failed) {
    return false;
  }

  nlohmann::json config = *inter->pro;

  for (auto &mode : config["modes"]) {
    if (mode["name"] == mode_name) {
      return true;
    }
  }

  Frate::Utils::error << "Failed to add mode : could not find " << mode_name
                      << " in frate-project.json" << std::endl;
  return true;
}
#endif
