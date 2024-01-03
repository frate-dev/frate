#include <Frate/Generators.hpp>
#include <Frate/Project.hpp>
#include <Frate/Template.hpp>
#include <fstream>
#include <inja.hpp>

namespace Frate::Generators::Project {
  bool loadTemplateConfig(std::shared_ptr<Command::Project> pro) {

    std::ifstream template_config_file;

    try {

      template_config_file.open(pro->template_path / "template.json");

    } catch (...) {

      Utils::error << "Error while opening file: "
                   << pro->template_path / "template.json" << std::endl;
      return false;
    }

    json j = json::parse(template_config_file);

    Command::Template template_config = j;
    pro->fromTemplate(template_config);
    return true;
  }
} // namespace Frate::Generators::Project
