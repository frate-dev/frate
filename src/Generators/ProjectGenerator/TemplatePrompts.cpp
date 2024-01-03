#include "Frate/Template.hpp"
#include <Frate/Generators.hpp>
#include <Frate/Project.hpp>
#include <Frate/TemplateMeta.hpp>

namespace Frate::Generators::Project {
  using Command::TemplateMeta;

  std::pair<bool, TemplateMeta> promptForTemplateName(json index) {
    Prompt template_name_prompt("Project type: ");
    for (TemplateMeta templ : index) {
      template_name_prompt.addOption(templ.name);
    }

    template_name_prompt.printValidOptions();

    template_name_prompt.run();

    auto [success, template_id] = template_name_prompt.get<std::string>();

    if (!success) {
      Utils::error << "Error while getting template name" << std::endl;
      return {false, TemplateMeta()};
    }

    TemplateMeta templ;
    for (TemplateMeta temp : index) {
      if (temp.name == template_id) {
        templ = temp;
        break;
      }
    }

    if (templ.name.empty()) {
      Utils::error << "TemplateMeta not found" << std::endl;
      return {false, TemplateMeta()};
    }
    return {true, templ};
  }

  bool runTemplatePrompts(std::shared_ptr<Command::Project> pro) {
    for (auto [key, tmpl_prompt] : pro->prompts) {
      Prompt prompt(tmpl_prompt.text, tmpl_prompt.default_value);
      if (tmpl_prompt.type == "bool") {
        prompt.isBool();
      }

      for (std::string option : tmpl_prompt.options) {
        prompt.addOption(option);
      }

      prompt.printValidOptions();

      prompt.run();
      auto [valid, value] = prompt.get<std::string>();
      if (!valid) {
        Utils::error << "Error while getting prompt: " << key << std::endl;
        return false;
      }
      pro->prompts[key].value = value;
    }
    return true;
  }
} // namespace Frate::Generators::Project
