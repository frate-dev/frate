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

    static void from_json(const json& j, Template& t){
      t.name = j.at("name").get<std::string>();
      t.git = j.at("git").get<std::string>();
      t.description = j.at("description").get<std::string>();
    }

    std::pair<bool, Template> promptForProjectName(json index);

    bool create(std::shared_ptr<Command::Project> pro);

    bool refresh(std::shared_ptr<Command::Project> pro);

    std::pair<bool, Template> promptForTemplateName(json index);

    bool renderTemplate(
        Environment &env,
        std::shared_ptr<Command::Project> pro);

  bool refreshTemplate(Environment &env, std::shared_ptr<Command::Project> pro);

    bool runTemplatePrompts(std::shared_ptr<Command::Project> pro);

    bool downloadTemplate(std::string git_url, path project_path);

    bool loadTemplateConfig(std::shared_ptr<Command::Project> pro);

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



