#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace Frate::Command {
  class TemplateMeta {
  public:
    TemplateMeta() = default;
    TemplateMeta(const nlohmann::json &json_obj);
    std::string name;
    std::string description;
    std::string hash;
    std::string git;
    friend void from_json(const nlohmann::json &json_obj, TemplateMeta &temp);
    friend void to_json(nlohmann::json &json_obj, const TemplateMeta &temp);
    friend std::ostream &operator<<(std::ostream &os_stream,
                                    const TemplateMeta &temp);
  };
} // namespace Frate::Command
