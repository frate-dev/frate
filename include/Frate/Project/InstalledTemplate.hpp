#include "Frate/Project/Config.hpp"
#include "Frate/System/GitCommit.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <utility>
#include <vector>

namespace Frate::Project {

  //   class InstalledTemplate {
  //   private:
  //     /*
  //      * This will be the latest commit hash, if getLatestCommit() is called
  //      more
  //      * than once, it will return the cached value
  //      */
  //     System::GitCommit latest;
  //
  //     std::string name;
  //     // Git url to template
  //     std::string git;
  //     // The latest commit hash
  //     std::vector<System::GitCommit> commits;
  //
  //     /*
  //      * Returns true if the template is installed
  //      * @param template_hash the hash of the template to check
  //      */
  //     bool validate_installed(std::string &template_hash);
  //
  //     TemplateConfig get_template_info(std::string &template_hash);
  //
  //     void render_template(std::shared_ptr<Config> config);
  //
  //   public:
  //     InstalledTemplate() = default;
  //
  //     // Returns the latest commit hash
  //     System::GitCommit &getLatestCommit();
  //     /*
  //      * Builds the template into the the project path
  //      * and transfers the template config to the project config
  //      * WARNING this will overwrite everything in the project path
  //      * @param config the project config to build the template into
  //      */
  //     void build(std::shared_ptr<Config> config);
  //     /*
  //      * Refreshes the template by rendering the CMakeLists.txt into the
  //      project
  //      * @param config the project config to refresh the template into
  //      */
  //     void refresh(std::shared_ptr<Config> config);
  //
  //     friend void to_json(nlohmann::json &json_obj,
  //                         const InstalledTemplate &_template);
  //     friend void from_json(const nlohmann::json &json_obj,
  //                           InstalledTemplate &_template);
  //     friend std::ostream &operator<<(std::ostream &os_stream,
  //                                     const InstalledTemplate &temp);
  //
  //     [[nodiscard]] std::string &getName() { return name; }
  //
  //     [[nodiscard]] std::string &getGit() { return git; }
  //
  //     [[nodiscard]] std::vector<System::GitCommit> &getCommits() {
  //       return commits;
  //     }
  //   };
} // namespace Frate::Project
