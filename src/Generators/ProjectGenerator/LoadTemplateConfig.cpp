#include <Frate/Generators.hpp>
#include <Frate/Project/Config.hpp>
#include <Frate/Project/Template.hpp>
#include <fstream>
#include <inja.hpp>

namespace Frate::Generators::Project {
  using ::Frate::Project::TemplateConfig;

  bool loadTemplateConfig(std::shared_ptr<Project::Config> pro) {

    std::ifstream template_config_file;

    try {

      template_config_file.open(pro->template_path / "template.json");

    } catch (...) {

      Utils::error << "Error while opening file: "
                   << pro->template_path / "template.json" << std::endl;
      return false;
    }

    json j = json::parse(template_config_file);

    TemplateConfig template_config = j;
    pro->fromTemplate(template_config);
    return true;
  }
} // namespace Frate::Generators::Project
