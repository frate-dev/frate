#pragma once
#include <Frate/Command.hpp>


namespace Frate::Command::License {
  typedef struct License_s {
    std::string name;
    std::string key;
    std::string url;
    std::string spdx_id;
  } License;

  typedef struct FullLicense_s {
    std::string key;
    std::string name;
    std::string spdx_id;
    std::string url; std::string html_url; std::string description;
    std::string implementation;
    std::vector<std::string> permissions;
    std::vector<std::string> conditions;
    std::vector<std::string> limitations;
    std::string body;
    bool featured;
  } FullLicense;

  void to_json(json& j, const License& l);

  void from_json(const json& j, License& l);

  void to_json(json& j, const FullLicense& l);

  void from_json(const json& j, FullLicense& l);

  /*
   * set the current projet's license
   */
  bool set(std::shared_ptr<Interface> inter);
  /*
   * remove the current project's license
   */
  bool remove(std::shared_ptr<Interface> inter);
  /*
   * list all available licenses
   */
  bool list(std::shared_ptr<Interface> inter);
}
