#include "Frate/Template/IndexEntry.hpp"
#include "Frate/Template/Renderer.hpp"
#include <Frate/Project/Config.hpp>
#include <vector>

namespace Frate::Project {

  class TemplateManager {

  private:
    std::vector<TemplateIndexEntry> index;
    std::string branch;

    std::unordered_map<std::string,
                       std::unordered_map<std::string, TemplateRenderer>>
        installed;

    bool index_loaded = false;
    /*
     * Loads the index from the github repo
     */
    void load_index();
    /*
     * Checks if a template is installed
     * @param name the name of the template to check
     * @param hash the hash of the template if it is installed
     * @return true if the template is installed
     */
    Project::TemplateRenderer &find_template(const std::string &name,
                                         std::string &hash);
    /*
     * Checks if a template specifed is installed
     * nice helper function
     * @param name the name of the template to check
     * @param hash the hash of the template if it is installed
     * @return true if the template is installed
     */
    bool is_installed(const std::string &name, std::string &hash);
    /*
     * Works similar to the other is_installed but will assume the branch of the TEMPLATE_BRANCH
     * @param template instance
     * @return true if the template is installed
     */
    bool is_installed(TemplateIndexEntry &entry);
    /*
     * Moves downloaded template to the installpath specified
     * @param tmp_path the path to the downloaded template
     * @param template_path the path to the template to install
     * @param hash the hash of the template to install
     */
    void template_to_installed_path(std::filesystem::path &tmp_path,
                                    std::filesystem::path &template_path,
                                    std::string &hash);

  public:
    TemplateManager(std::string branch) : branch(branch) {}
    ~TemplateManager() = default;
    /*
     * Installs a template based on the local index, if hash is not provided then it will defult to the latest hash
     */
    TemplateRenderer install(TemplateIndexEntry &entry);
    /*
     * Install the local latest based on the latest hash in the index is
     * installed
     * If there is no templates installed for the git url specified name then it will
     * install that template
     * @param git url of the template to install
     */
    TemplateRenderer getLatest(std::string &git_url);
    TemplateRenderer get(std::string &name,std::string &hash);
    TemplateRenderer promptList();
    /*
     * Uninstalls a template
     * @param name the name of the template to uninstall
     */
    void uninstall(std::string &name);
    /*
     * Updates to the latest version of the template
     * @param name the name of the template to update
     */
    void update(std::string &name);

    void updateIndex();

    void installAll();

    /*
     * Creates a tempalte in a tmp path and then returns the path that rendering
     * is supposed to happen in
     * @param override_path the path to override the template path with
     * @param name the name of the template to create
     * @param hash the hash of the template to create
     */
    // std::filesystem::path makeTemplate(const std::filesystem::path
    // &override_path,const std::string &name, std::string &hash);

    void save(); // Saves this object to the config file
    void load(); // Loads this object from the config file
    /*
     * Grabs the current installed templates from the config file
     * @return a vector of TemplateMeta objects
     */
    std::unordered_map<std::string,
                       std::unordered_map<std::string, TemplateRenderer>> &
    getInstalled();
    /*
     * Grabs the current index from the the github repo, if called more than
     * once then it will pull the index from memory
     * @return a vector of TemplateMeta objects
     */
    std::vector<TemplateIndexEntry> &getIndex();

    std::string& getBranch();

    friend void from_json(const nlohmann::json &json_obj,
                          TemplateManager &config);

    friend void to_json(nlohmann::json &json_obj,
                        const TemplateManager &config);
  };
} // namespace Frate::Project
