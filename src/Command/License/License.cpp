#include <Frate/Command/License.hpp>
#include <Frate/Utils/Macros.hpp>

namespace Frate::Command::License {

  void to_json(json &json_obj, const License &license) {
    TO_JSON_FIELD(license, name);
    TO_JSON_FIELD(license, key);
    TO_JSON_FIELD(license, url);
    TO_JSON_FIELD(license, spdx_id);
  }

  void from_json(const json &json_obj, License &license) {
    FROM_JSON_FIELD(license, name);
    FROM_JSON_FIELD(license, key);
    FROM_JSON_FIELD(license, url);
    FROM_JSON_FIELD(license, spdx_id);
  }

  void to_json(json &json_obj, const FullLicense &license) {
    TO_JSON_FIELD(license, key);
    TO_JSON_FIELD(license, name);
    TO_JSON_FIELD(license, spdx_id);
    TO_JSON_FIELD(license, url);
    TO_JSON_FIELD(license, html_url);
    TO_JSON_FIELD(license, description);
    TO_JSON_FIELD(license, implementation);
    TO_JSON_FIELD(license, permissions);
    TO_JSON_FIELD(license, conditions);
    TO_JSON_FIELD(license, limitations);
    TO_JSON_FIELD(license, body);
    TO_JSON_FIELD(license, featured);
  }

  void from_json(const json &json_obj, FullLicense &license) {
    FROM_JSON_FIELD(license, key);
    FROM_JSON_FIELD(license, name);
    FROM_JSON_FIELD(license, spdx_id);
    FROM_JSON_FIELD(license, url);
    FROM_JSON_FIELD(license, html_url);
    FROM_JSON_FIELD(license, description);
    FROM_JSON_FIELD(license, implementation);
    FROM_JSON_FIELD(license, permissions);
    FROM_JSON_FIELD(license, conditions);
    FROM_JSON_FIELD(license, limitations);
    FROM_JSON_FIELD(license, body);
    FROM_JSON_FIELD(license, featured);
  }

} // namespace Frate::Command::License
