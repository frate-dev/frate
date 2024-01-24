#include <Frate/Project/Config.hpp>
#include <Frate/Template/Config.hpp>
#include <Frate/Utils/Logging.hpp>
#include <Frate/Utils/Macros.hpp>

namespace Frate::Project {

  TemplateConfig::TemplateConfig() = default;

  void from_json(const json &json_obj, TemplateConfig &template_) {
    FROM_JSON_FIELD(template_, name);
    FROM_JSON_FIELD(template_, description);
    FROM_JSON_FIELD(template_, version);
    FROM_JSON_FIELD(template_, src_dir);
    FROM_JSON_FIELD(template_, include_dir);
    FROM_JSON_FIELD(template_, build_dir);
    FROM_JSON_FIELD(template_, dependencies);
    FROM_JSON_FIELD(template_, default_mode);
    FROM_JSON_FIELD(template_, modes);
    FROM_JSON_FIELD(template_, flags);
    FROM_JSON_FIELD(template_, system_libs);
    FROM_JSON_FIELD(template_, supported_toolchains);
    FROM_JSON_FIELD(template_, supported_languages);
    FROM_JSON_FIELD(template_, supported_language_versions);
    FROM_JSON_FIELD(template_, min_cmake_version);
    FROM_JSON_FIELD(template_, supported_compilers);
    FROM_JSON_FIELD(template_, keywords);
    FROM_JSON_FIELD(template_, prompts);
    FROM_JSON_FIELD(template_, global);
  }

  void to_json(json &json_obj, const TemplateConfig &template_) {
    Utils::info << "Template to json" << std::endl;
    TO_JSON_FIELD(template_, name);
    TO_JSON_FIELD(template_, description);
    TO_JSON_FIELD(template_, version);
    TO_JSON_FIELD(template_, src_dir);
    TO_JSON_FIELD(template_, include_dir);
    TO_JSON_FIELD(template_, build_dir);
    TO_JSON_FIELD(template_, dependencies);
    TO_JSON_FIELD(template_, default_mode);
    TO_JSON_FIELD(template_, modes);
    TO_JSON_FIELD(template_, flags);
    TO_JSON_FIELD(template_, system_libs);
    TO_JSON_FIELD(template_, supported_toolchains);
    TO_JSON_FIELD(template_, supported_languages);
    TO_JSON_FIELD(template_, supported_language_versions);
    TO_JSON_FIELD(template_, min_cmake_version);
    TO_JSON_FIELD(template_, supported_compilers);
    TO_JSON_FIELD(template_, keywords);
    TO_JSON_FIELD(template_, prompts);
    TO_JSON_FIELD(template_, global);
  }

  void to_project(Project::Config &pro, const TemplateConfig &template_) {
    Utils::info << "Template to project" << std::endl;
    pro.name = template_.name;
    pro.description = template_.description;
    pro.version = template_.version;
    pro.src_dir = template_.src_dir;
    pro.include_dir = template_.include_dir;
    pro.build_dir = template_.build_dir;
    pro.dependencies = template_.dependencies;
    pro.default_mode = template_.default_mode;
    pro.modes = template_.modes;
    Utils::info << "Template to project end" << std::endl;
  }
}; // namespace Frate::Project
