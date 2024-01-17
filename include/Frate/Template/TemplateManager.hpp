#include "Frate/Template/TemplateMeta.hpp"
#include <Frate/Template/InstalledTemplate.hpp>
#include <variant>
#include <vector>

namespace Frate {
  class TemplateManager {
  private:
    std::vector<Command::TemplateMeta> index;
    std::vector<Project::InstalledTemplate> installed;
    bool index_loaded = false;
    /*
     * Loads the index from the github repo
     */
    bool load_index();
    /*
     * Checks if a template is installed
     * @param name the name of the template to check
     * @param hash the hash of the template if it is installed
     * @return true if the template is installed
     */
    Project::InstalledTemplate &find_template(const std::string &name);
    bool is_installed(const std::string &name, std::string &hash);

    bool template_to_installed_path(std::filesystem::path &tmp_path,
                                    std::filesystem::path &template_path,
                                    std::string &hash);

  public:
    TemplateManager() = default;
    ~TemplateManager() = default;
    /*
     * Installs a template based on the index
     */
    Command::TemplateMeta install(const std::string &name,
                                  std::string hash = "");
    /*
     * Uninstalls a template
     * @param name the name of the template to uninstall
     */
    bool uninstall(const std::string &name);
    /*
     * Updates to the latest version of the template
     * @param name the name of the template to update
     */
    bool update(const std::string &name);
    /*
     * Creates a tempalte in a tmp path and then returns the path that rendering
     * is supposed to happen in
     * @param override_path the path to override the template path with
     * @param name the name of the template to create
     * @param hash the hash of the template to create
     */
    std::filesystem::path
    makeTemplate(const std::filesystem::path &override_path,
                 const std::string &name, std::string &hash);
    /*
     * Grabs the current installed templates from the config file
     * @return a vector of TemplateMeta objects
     */
    std::vector<Project::InstalledTemplate> &getInstalled();
    /*
     * Grabs the current index from the the github repo, if called more than
     * once then it will pull the index from memory
     * @return a vector of TemplateMeta objects
     */
    std::vector<Command::TemplateMeta> &getIndex();

    friend void from_json(const nlohmann::json &json_obj,
                          TemplateManager &config);

    friend void to_json(nlohmann::json &json_obj,
                        const TemplateManager &config);
  };
} // namespace Frate
