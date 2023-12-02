#include <Frate/Command/License.hpp>
namespace Frate::Command::License { 

  void to_json(json& j, const License& l) {
    j = json{
      {"name", l.name},
      {"key", l.key},
      {"url", l.url},
      {"spdx_id", l.spdx_id}
    };
  }

  void from_json(const json& j, License& l) {
    j.at("name").get_to(l.name);
    j.at("key").get_to(l.key);
    j.at("url").get_to(l.url);
    j.at("spdx_id").get_to(l.spdx_id);
  }

  void to_json(json& j, const FullLicense& l) {
    j = json{
      {"key", l.key},
      {"name", l.name},
      {"spdx_id", l.spdx_id},
      {"url", l.url},
      {"html_url", l.html_url},
      {"description", l.description},
      {"implementation", l.implementation},
      {"permissions", l.permissions},
      {"conditions", l.conditions},
      {"limitations", l.limitations},
      {"body", l.body},
      {"featured", l.featured}
    };
  }

  void from_json(const json& j, FullLicense& l) {
    j.at("key").get_to(l.key);
    j.at("name").get_to(l.name);
    j.at("spdx_id").get_to(l.spdx_id);
    j.at("url").get_to(l.url);
    j.at("html_url").get_to(l.html_url);
    j.at("description").get_to(l.description);
    j.at("implementation").get_to(l.implementation);
    j.at("permissions").get_to(l.permissions);
    j.at("conditions").get_to(l.conditions);
    j.at("limitations").get_to(l.limitations);
    j.at("body").get_to(l.body);
    j.at("featured").get_to(l.featured);
  }

}
