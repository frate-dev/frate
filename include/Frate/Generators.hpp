#include <memory>
#include <vector>
#include "./Command.hpp"
#include <inja.hpp>

namespace Frate::Generators{
  namespace Project {
    using nlohmann::json;
    using Utils::CLI::Prompt;
    using nlohmann::json;
    using std::filesystem::path;
    using Utils::CLI::Prompt;
    using inja::Environment;
    json getTemplateIndex();
typedef struct Template {
      std::string name;
      std::string git;
      std::string description;
    } Template;


    /*
     * ===========================
     * External functions
     * ===========================
     */
    
    /*
     * This is the external callback used when ever a new project is created
     * @param pro: the project context
     * @return true if the creation was successful
     */
    bool create(std::shared_ptr<Command::Project> pro);

    /*
     * This is the external callback used when ever the tempalte needs to be refreshed
     * @param pro: the project context
     * @return true if the refresh was successful
     */
    bool refresh(std::shared_ptr<Command::Project> pro);



    /*
     * ===========================
     * Internal functions
     * ===========================
     */

    /*
     * Impliceitly converts a json object to a Template object
     * Intended to to convert the json objects from the template index to Template objects
     * @param j: the json object
     * @param t: the template object
     */
    void from_json(const json& j, Template& t);
    /*
     * Prompts the user for a template name, only used if the template has not been specified
     * @param index: the json index of the templates
     * @return a pair of bool and Template, the bool is true if the template was found
     * and the Template is the template that was found
     */
    std::pair<bool, Template> promptForTemplateName(json index);
    
    /*
     * Renders the entire template, this is only used when creating a new project
     * @param env: the inja environment
     * @param pro: the project context
     * @return true if the render was successful
     */
    bool renderTemplate(Environment &env, std::shared_ptr<Command::Project> pro);

    /*
     * Refreshes only specific files that are considerer dynamic files
     * @param env: the inja environment
     * @param pro: the project context
     * @return true if the refresh was successful
     */
    bool refreshTemplate(Environment &env, std::shared_ptr<Command::Project> pro);
    
    /*
     * Runs user prompts from the template
     * @param pro: the project context
     * @return true if the prompts were successful
     */
    bool runTemplatePrompts(std::shared_ptr<Command::Project> pro);

    /*
     * Downloads the templates from from the github repo
     * @param git_url: the url of the git repo
     * @param project_path: the path of the project
     * @return true if the download was successful
     */
    bool downloadTemplate(std::string git_url, path project_path);

    /*
     * Loads the template config file and merges into the project context
     * @param pro: the project context
     * @return true if the load was successful
     */
    bool loadTemplateConfig(std::shared_ptr<Command::Project> pro);
    
    /*
     * Initializes the lua environment after by loading user scripts into the inja environment
     * @param env: the inja environment
     * @param lua: the lua state
     * @param pro: the project context
     * @return true if the initialization was successful
     */
    bool initializeLua(Environment &env, sol::state &lua, std::shared_ptr<Command::Project> pro);
  }
  namespace GitIgnore{
    bool create(std::shared_ptr<Command::Interface> inter);
  }
  namespace Readme{
    bool create(std::shared_ptr<Command::Interface> inter);
  }
  namespace Toolchain{
    std::string generateToolchain(std::string toolchain);
  }
  namespace CMakeList{
    typedef struct Dep {
      std::string fetch_declare;
      std::string fetch_make_available;
      std::string target_link_libraries;
    } Dep;
    typedef struct CMakeContext {
      std::string cmake_minimum_required;
      std::string project_name;
      std::string project_version;
      std::string build_type;
      std::string cxx_version;
      std::string compiler;
      std::string source_dir;
      std::string build_dir;
      std::string fetch_content;
      std::string include_fetch;
      std::string files;
      std::string include_dir;
      std::string add_executable;
      std::string testing;
      std::string mode;
      std::string set_build_dir;
      std::vector<Dep> dependencies;
    } CMakeContext;
    /*
     * Generate the dependencies for the project
     * @param ctx: the context of the command
     * @return a vector of dependencies that will be later combined to build the cmake file
     */

    bool create(std::shared_ptr<Command::Project> pro); 
    bool createCMakeLists(std::shared_ptr<Command::Project> pro);
  }

  namespace ConfigJson{

    typedef struct Config_s{
      std::string cmake_version;
      std::string project_name;
      std::string project_version;
      std::string include_dir;
      std::string compiler;
      std::string name;
      std::string src_dir;
      std::string build_dir;
      std::string lang;
      std::string lang_version;
      std::string authors_str;
    } Config;
    [[deprecated("Use project->save() instead")]]
    bool writeConfig(std::shared_ptr<Command::Project>& ctx);
  }

}



