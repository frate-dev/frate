
#include "Frate/Interface.hpp"
#include "Frate/LuaAPI.hpp"
#include "Frate/System/Build.hpp"
#include "Frate/Utils/General.hpp"
#include "inja.hpp"
#include <Frate/Constants.hpp>
#include <Frate/Generators.hpp>
#include <Frate/Project.hpp>
#include <Frate/TemplateMeta.hpp>
#include <filesystem>

namespace Frate::Generators::Project {
  using Command::TemplateMeta;

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

    Command::TemplateMeta current_template;

    for (TemplateMeta templ : index) {
      if (inter->pro->type == templ.name) {
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
      Utils::info << "Creating project from template: " << templ.name
                  << std::endl;
    }

    std::filesystem::create_directories(inter->pro->path / "override");

    TemplateMeta installed_template =
        inter->config.templates.install(current_template.name);

    const std::filesystem::path override_path = inter->pro->path / "override";

    std::filesystem::path render_path = inter->config.templates.makeTemplate(
        override_path, installed_template.name, installed_template.hash);

    inter->pro->template_path = render_path;
    inter->pro->current_template = installed_template;

    Utils::info << "Installed template" << installed_template << std::endl;

    Utils::info << "Copying template to project" << std::endl;

    std::filesystem::copy(
        inter->pro->template_path, inter->pro->path,
        std::filesystem::copy_options::recursive |
            std::filesystem::copy_options::overwrite_existing);

    Utils::info << "Loading template config" << std::endl;
    if (!loadTemplateConfig(inter->pro)) {
      Utils::error << "Error while loading template config" << std::endl;
      return false;
    }
    Utils::info << "Running template prompts" << std::endl;
    if (!runTemplatePrompts(inter->pro)) {
      Utils::error << "Error while running template prompts" << std::endl;
      return false;
    }

    inja::Environment env;
    sol::state lua;

    Utils::info << "Initializing lua" << std::endl;
    if (!initializeLua(env, lua, inter->pro)) {
      Utils::error << "Error while initializing lua" << std::endl;
      return false;
    }

    LuaAPI::initScripts(lua, inter->pro);

    Utils::info << "Rendering template" << std::endl;
    if (!renderTemplate(env, inter->pro)) {
      Utils::error << "Error while rendering template to tmp" << std::endl;
      return false;
    }

    LuaAPI::postScripts(lua, inter->pro);

    Utils::info << "Copying rendered template to project" << std::endl;

    std::filesystem::copy(
        render_path, inter->pro->path,
        std::filesystem::copy_options::recursive |
            std::filesystem::copy_options::overwrite_existing);

    // This ia bit of a hack, because create doesn't actually load a project so
    // we have to emulate the loading process
    inter->pro->loaded_json = true;
    inter->pro->save();

    std::filesystem::remove_all(render_path);

    return true;
  }

  bool refresh(std::shared_ptr<Command::Interface> inter) {
    inja::Environment env;
    sol::state lua;

    const std::filesystem::path override_path = inter->pro->path / "override";

    std::filesystem::path render_path = inter->config.templates.makeTemplate(
        override_path, inter->pro->current_template.name,
        inter->pro->current_template.hash);

    inter->pro->template_path = render_path;

    if (!initializeLua(env, lua, inter->pro)) {
      Utils::error << "Error while initializing lua" << std::endl;
      return false;
    }

    Utils::verbose << "Initializing lua scripts at: "
                   << inter->pro->path / "template/scripts" << std::endl;
    LuaAPI::initScripts(lua, inter->pro);

    Utils::verbose << "Refreshing template" << std::endl;
    if (!refreshTemplate(env, inter->pro)) {
      Utils::error << "Error while rendering template to tmp" << std::endl;
      return false;
    }

    LuaAPI::postScripts(lua, inter->pro);

    Utils::verbose << "Copying rendered template to project" << std::endl;

    std::filesystem::copy(
        render_path, inter->pro->path,
        std::filesystem::copy_options::recursive |
            std::filesystem::copy_options::overwrite_existing);

    std::filesystem::remove_all(render_path);

    return true;
  }
} // namespace Frate::Generators::Project
