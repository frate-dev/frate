#include "Frate/Project/TemplateIndexEntry.hpp"
#include "Frate/System/Build.hpp"
#include "Frate/Utils/General.hpp"
#include <Frate/Constants.hpp>
#include <Frate/Generators.hpp>
#include <Frate/Project/Config.hpp>
#include <Frate/Project/Exceptions.hpp>
#include <Frate/Project/TemplateMeta.hpp>
#include <filesystem>

namespace Frate::Generators::Project {

  json getTemplateIndex() {
    std::cout << "Getting Template Index" << std::endl;
    std::string index_url = Constants::TEMPLATE_INDEX_URL;

    json index = json::parse(Utils::fetchText(index_url));
    return index;
  }

  bool create(std::shared_ptr<Command::Interface> inter) {
    Utils::info << "Creating Project" << std::endl;

    json index = getTemplateIndex();
    bool has_template = false;

    Project::TemplateMeta current_template;

    for (TemplateIndexEntry templ : index) {
      if (inter->pro->type == templ.getName()) {
        has_template = true;
        current_template = templ;
        break;
      }
    }

    if (!has_template) {
      auto [success, templ] = promptForTemplateName(index);
      if (!success) {
        Utils::error << "Error while prompting for project name" << std::endl;
        return false;
      }
      current_template = TemplateMeta(templ);
      Utils::info << "Creating project from template: " << templ.getName()
                  << std::endl;
    }

    std::filesystem::create_directories(inter->pro->path / "override");

    inter->pro->current_template =
        inter->templates->install(current_template.getName());

    Utils::verbose << "Installed template: "
                   << inter->pro->current_template.getName()
                   << " with hash of: "
                   << inter->pro->current_template.getHash() << std::endl;

    inter->pro->current_template.build(inter->pro);

    inter->pro->loaded_json = true;
    inter->pro->save();

    return true;
  }

  bool refresh(std::shared_ptr<Command::Interface> inter) {
    inter->pro->current_template.refresh(inter->pro);
    return true;
  }
} // namespace Frate::Generators::Project
