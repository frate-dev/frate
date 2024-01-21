#pragma once
#include <Frate/Interface.hpp>
#include <Frate/Project/Config.hpp>
#include <Frate/Project/TemplateMeta.hpp>
#include <Frate/Utils/CLIPrompt.hpp>
#include <inja.hpp>
#include <vector>

namespace Frate::Generators {
//   namespace Project {
//     using ::Frate::Project::Config;
//     using ::Frate::Project::TemplateIndexEntry;
//     using ::Frate::Project::TemplateMeta;
//     using inja::Environment;
//     using nlohmann::json;
//     using std::filesystem::path;
//     using Utils::CLI::Prompt;
//     json getTemplateIndex();
// 
//     /*
//      * ===========================
//      * External functions
//      * ===========================
//      */
// 
//     /*
//      * This is the external callback used when ever a new project is created
//      * @param pro: the project context
//      * @return true if the creation was successful
//      */
//     bool create(std::shared_ptr<Command::Interface> inter);
// 
//     /*
//      * This is the external callback used when ever the tempalte needs to be
//      * refreshed
//      * @param pro: the project context
//      * @return true if the refresh was successful
//      */
//     bool refresh(std::shared_ptr<Command::Interface> inter);
// 
//     /*
//      * ===========================
//      * Internal functions
//      * ===========================
//      */
// 
//     /*
//      * Prompts the user for a template name, only used if the template has not
//      * been specified
//      * @param index: the json index of the templates
//      * @return a pair of bool and Template, the bool is true if the template was
//      * found and the Template is the template that was found
//      */
//     //std::pair<bool, TemplateMeta> promptForTemplateName(json index);
// 
//     /*
//      * Renders the entire template, this is only used when creating a new
//      * project
//      * @param env: the inja environment
//      * @param pro: the project context
//      * @return true if the render was successful
//      */
//     bool renderTemplate(Environment &env, std::shared_ptr<Project::Config> pro);
//     /*
//      * Refreshes only specific files that are considerer dynamic files
//      * @param env: the inja environment
//      * @param pro: the project context
//      * @return true if the refresh was successful
//      */
//     bool refreshTemplate(Environment &env,
//                          std::shared_ptr<Project::Config> pro);
// 
//     /*
//      * Runs user prompts from the template
//      * @param pro: the project context
//      * @return true if the prompts were successful
//      */
//     //bool runTemplatePrompts(std::shared_ptr<Project::Config> pro);
// 
//     /*
//      * Downloads the templates from from the github repo
//      * @param git_url: the url of the git repo
//      * @param project_path: the path of the project
//      * @return true if the download was successful
//      */
//     bool downloadTemplate(std::string template_name,
//                           std::shared_ptr<Command::Interface> inter);
// 
//     /*
//      * Loads the template config file and merges into the project context
//      * @param pro: the project context
//      * @return true if the load was successful
//      */
//     bool loadTemplateConfig(std::shared_ptr<Project::Config> pro);
// 
//     /*
//      * Initializes the lua environment after by loading user scripts into the
//      * inja environment
//      * @param env: the inja environment
//      * @param lua: the lua state
//      * @param pro: the project context
//      * @return true if the initialization was successful
//      */
//     bool initializeLua(Environment &env, sol::state &lua,
//                        std::shared_ptr<Project::Config> pro);
//   } // namespace Project

  namespace GitIgnore {
    bool create(std::shared_ptr<Command::Interface> inter);
  }

  namespace Readme {
    bool create(std::shared_ptr<Command::Interface> inter);
  }

  namespace Toolchain {
    std::string generateToolchain(std::string toolchain);
  }

  namespace DockerTemplate {
    bool create(std::shared_ptr<Command::Interface> inter);
    bool remove(std::shared_ptr<Command::Interface> inter);
  } // namespace DockerTemplate

} // namespace Frate::Generators
