#include "nlohmann/json_fwd.hpp"
#include <Frate/Command/License.hpp>
#include <Frate/Utils/General.hpp>

namespace Frate::Command::License {
using nlohmann::json;

bool list(std::shared_ptr<Interface> inter) {
  Utils::Info info;
  json github_licenses = Utils::fetchJson("https://api.github.com/licenses");

  for (License license : github_licenses) {
    Utils::info << license.name << std::endl;
  }

  return true;
  }
}
