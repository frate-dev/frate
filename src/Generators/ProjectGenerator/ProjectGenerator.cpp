
#include "Frate/Interface.hpp"
#include "Frate/LuaAPI.hpp"
#include "Frate/Utils/General.hpp"
#include "inja.hpp"
#include <Frate/Constants.hpp>
#include <Frate/Generators.hpp>
#include <Frate/Project.hpp>
#include <filesystem>

namespace Frate::Generators::Project {
  void from_json(const json &j, Template &t) {
    t.name = j.at("name").get<std::string>();
    t.git = j.at("git").get<std::string>();
    t.description = j.at("description").get<std::string>();
  }

  json getTemplateIndex() {
    std::cout << "Getting Template Index" << std::endl;
    std::string index_url =
        static_cast<std::string>(Constants::FRATE_TEMPLATES);

    json index = json::parse(Utils::fetchText(index_url));
    return index;
  }

  bool create(std::shared_ptr<Command::Project> pro) {
    Utils::info << "Creating Project" << std::endl;

    json index = getTemplateIndex();
    bool has_template = false;

    Template current_template;

    for (Template templ : index) {
      if (pro->type == templ.name) {
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
      current_template = templ;
      Utils::info << "Creating project from template: " << templ.name
                  << std::endl;
    }
    Utils::info << "Downloading template at: " << current_template.git
                << std::endl;
    if (!downloadTemplate(current_template.git, pro->path)) {
      Utils::error << "Error while downloading template" << std::endl;
      return false;
    }

    Utils::info << "Copying template to project" << std::endl;
    std::filesystem::copy(
        pro->path / "template", pro->path,
        std::filesystem::copy_options::recursive |
            std::filesystem::copy_options::overwrite_existing);
    Utils::info << "Loading template config" << std::endl;
    if (!loadTemplateConfig(pro)) {
      Utils::error << "Error while loading template config" << std::endl;
      return false;
    }
    Utils::info << "Running template prompts" << std::endl;
    if (!runTemplatePrompts(pro)) {
      Utils::error << "Error while running template prompts" << std::endl;
      return false;
    }

    inja::Environment env;
    sol::state lua;

    Utils::info << "Initializing lua" << std::endl;
    if (!initializeLua(env, lua, pro)) {
      Utils::error << "Error while initializing lua" << std::endl;
      return false;
    }

    LuaAPI::initScripts(lua, pro);

    Utils::info << "Rendering template" << std::endl;
    if (!renderTemplate(env, pro)) {
      Utils::error << "Error while rendering template to tmp" << std::endl;
      return false;
    }

    LuaAPI::postScripts(lua, pro);

    // This ia bit of a hack, because create doesn't actually load a project so
    // we have to emulate the loading process
    pro->loaded_json = true;
    pro->save();

    return true;
  }

  bool refresh(std::shared_ptr<Command::Project> pro) {
    inja::Environment env;
    sol::state lua;

    if (!initializeLua(env, lua, pro)) {
      Utils::error << "Error while initializing lua" << std::endl;
      return false;
    }

    Utils::verbose << "Initializing lua scripts at: "
                   << pro->path / "template/scripts" << std::endl;
    LuaAPI::initScripts(lua, pro);

    Utils::verbose << "Refreshing template" << std::endl;
    if (!refreshTemplate(env, pro)) {
      Utils::error << "Error while rendering template to tmp" << std::endl;
      return false;
    }

    LuaAPI::postScripts(lua, pro);

    return true;
  }
} // namespace Frate::Generators::Project
