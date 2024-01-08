#include <Frate/Project/ProjectPrompt.hpp>
#include <Frate/Utils/Logging.hpp>
#include <Frate/Utils/Macros.hpp>

namespace Frate::Project {

  void to_json(json &json_obj, const ProjectPrompt &project_prompt) {
    TO_JSON_FIELD(project_prompt, text);
    TO_JSON_FIELD(project_prompt, value);
    TO_JSON_FIELD(project_prompt, default_value);
    TO_JSON_FIELD(project_prompt, type);
    TO_JSON_FIELD(project_prompt, required);
  }

  void from_json(const json &json_obj, ProjectPrompt &project_prompt) {
    FROM_JSON_FIELD(project_prompt, text);
    FROM_JSON_FIELD(project_prompt, value);
    FROM_JSON_FIELD(project_prompt, default_value);
    FROM_JSON_FIELD(project_prompt, type);
    FROM_JSON_FIELD(project_prompt, required);
    FROM_JSON_FIELD(project_prompt, options);
  }

  template <> std::string ProjectPrompt::get<std::string>() {

    if (this->type != "string") {
      Utils::error
          << "Cannot get string from non-string option in project options"
          << std::endl;
      exit(1);
    }

    if (!this->validator(this->value)) {
      if (this->required) {
        Utils::error << "Invalid value for required option: " << this->value
                     << std::endl;
        exit(1);
      }
      return this->default_value;
    }

    return this->value;
  }

  template <> bool ProjectPrompt::get<bool>() {

    if (this->type != "bool") {
      Utils::error << "Cannot get bool from non-bool option in project options"
                   << std::endl;
      exit(1);
    }

    if (!this->validator(this->value))
      return false;

    if (this->value == "true" || this->value == "yes" || this->value == "1")
      return true;

    return false;
  }

  template <> float ProjectPrompt::get<float>() {

    if (this->type != "float") {
      Utils::error
          << "Cannot get float from non-float option in project options"
          << std::endl;
      exit(1);
    }

    if (!this->validator(this->value)) {
      if (this->required) {
        Utils::error << "Invalid value for required option: " << this->value
                     << std::endl;
        exit(1);
      }
      return std::stof(this->default_value);
    }

    return std::stof(this->value);
  }

  template <> int ProjectPrompt::get<int>() {

    if (this->type != "int") {
      Utils::error << "Cannot get int from non-int option in project options"
                   << std::endl;
      exit(1);
    }

    if (!this->validator(this->value)) {
      if (this->required) {
        Utils::error << "Invalid value for required option: " << this->value
                     << std::endl;
        exit(1);
      }
      return std::stoi(this->default_value);
    }

    return std::stoi(this->value);
  }
} // namespace Frate::Project
