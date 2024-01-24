#include "Frate/Utils/Crypto/md5.hpp"
#include <Frate/Project/Cache.hpp>
#include <Frate/Project/Config.hpp>
#include <Frate/Utils/Logging.hpp>
#include <Frate/Utils/Macros.hpp>
#include <openssl/md5.h>
#include <sstream>

namespace Frate::Project {
  void from_json(const nlohmann::json &json_obj, Cache &cache_obj) {
    FROM_JSON_FIELD(cache_obj, build_command);
    FROM_JSON_FIELD(cache_obj, test_command);
    FROM_JSON_FIELD(cache_obj, run_command);
    FROM_JSON_FIELD(cache_obj, override_change_hash);
  }

  void to_json(nlohmann::json &json_obj, const Cache &cache_obj) {
    TO_JSON_FIELD(cache_obj, build_command);
    TO_JSON_FIELD(cache_obj, test_command);
    TO_JSON_FIELD(cache_obj, run_command);
    TO_JSON_FIELD(cache_obj, override_change_hash);
  }

  void Cache::generateOverrideChangeHash() {

    Utils::MD5Encoder encoder;

    std::stringstream entry_time;

    for (auto &dir_entry : std::filesystem::recursive_directory_iterator(
             this->config->path / "override")) {
        entry_time << dir_entry.last_write_time().time_since_epoch().count();
    }
    std::string entry_time_str = entry_time.str();
    this->override_change_hash = encoder.intake(entry_time_str).getDigest();
  }
} // namespace Frate::Project
