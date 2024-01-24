#pragma once
#include "Frate/Constants.hpp"
#include "Frate/Template/IndexEntry.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
namespace Frate::Lua {
  class TemplateEnvironment;
}

namespace Frate::Project {
  class Config;

  class TemplateMeta {
  private:
    std::string name;
    std::string description;
    std::string hash;
    std::string branch;
    std::string git;
    std::filesystem::path install_path;
    std::shared_ptr<Lua::TemplateEnvironment> env;
    bool scripts_loaded{false};
    /*
     * Contains the mapping of the template and the relative path to the project
     * path
     */
    std::unordered_map<std::filesystem::path, std::filesystem::path> file_map{};


    /*
     * loads scripts from __init__ , __post__ , and scripts from the template
     * into the template environment
     */
    void load_scripts();
    /*
     * Middleware function to render .inja files in the project path
     * after it's rendered the template files then it will delete the .inja
     * files
     * @param config the project config to render the template into
     */
    void render(std::shared_ptr<Config> config);

    void install_cpm(std::shared_ptr<Config> config);

    /*
     * Runs the prompts from the template and updates from the template config and sets the values
     * in the project config
     * @param config the project config to run the prompts into
     */
    void run_prompts(std::shared_ptr<Config> config);

  public:
    TemplateMeta();
    TemplateMeta(const nlohmann::json &json_obj);
    TemplateMeta(TemplateIndexEntry &entry);
    friend void from_json(const nlohmann::json &json_obj, TemplateMeta &temp);
    friend void to_json(nlohmann::json &json_obj, const TemplateMeta &temp);
    friend std::ostream &operator<<(std::ostream &os_stream,
                                    const TemplateMeta &temp);
    /*
     * Builds the template into the the project path
     * and transfers the template config to the project config
     * WARNING this will overwrite everything in the project path
     * @param config the project config to build the template into
     */
    void build(std::shared_ptr<Config> config);
    /*
     * Refreshes the template by rendering the CMakeLists.txt into the
     project
     * @param config the project config to refresh the template into
     */
    void refresh(std::shared_ptr<Config> config);

    // Getters

    std::vector<std::filesystem::path> &getTemplateFiles();

    std::string &getName() { return this->name; }

    std::string &getDescription() { return this->description; }

    std::string &getHash() { return this->hash; }

    std::string &getGit() { return this->git; }

    std::filesystem::path &getInstallPath() { return this->install_path; }

    void setName(std::string name) {
      this->install_path =
          Constants::INSTALLED_TEMPLATE_PATH / name / this->hash;
      this->name = name;
    }

    void setDescription(std::string description) {
      this->description = description;
    }

    void setHash(std::string hash) {
      this->install_path =
          Constants::INSTALLED_TEMPLATE_PATH / this->name / hash;
      this->hash = hash;
    }

    void setGit(std::string git) { this->git = git; }
  };
} // namespace Frate::Project
