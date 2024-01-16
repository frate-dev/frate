#include "Frate/Constants.hpp"
#include "Frate/System/GitCommit.hpp"
#include "Frate/Utils/Macros.hpp"
#include <Frate/Project/InstalledTemplate.hpp>
#include <Frate/Utils/Logging.hpp>
#include <fstream>

namespace Frate::Project {
  //   System::GitCommit &InstalledTemplate::getLatestCommit() {
  //     if (&latest == nullptr) {
  //       // TODO: Iterrate through commits and find the latest
  //     }
  //     return latest;
  //   }
  //
  //   bool InstalledTemplate::validate_installed(std::string &template_hash) {
  //
  //     if (template_hash.empty()) {
  //       Utils::error << "Template hash is empty";
  //       return false;
  //     }
  //
  //     return false;
  //   }
  //
  //   TemplateConfig
  //   InstalledTemplate::get_template_info(std::string &template_hash) {
  //
  //     std::filesystem::path template_config_path =
  //         Constants::INSTALLED_TEMPLATE_PATH / template_hash /
  //         "template.json";
  //
  //     if (!std::filesystem::exists(template_config_path)) {
  //       throw TemplateNotInstalled(("Template config file does not exist at:
  //       " +
  //                                   template_config_path.string()));
  //     }
  //
  //     json template_json;
  //     std::ifstream template_file;
  //
  //     try {
  //
  //       template_file = std::ifstream(Constants::INSTALLED_TEMPLATE_PATH /
  //                                     template_hash / "template.json");
  //
  //     } catch (std::exception &e) {
  //       throw std::runtime_error("Failed to open template config file at " +
  //                                Constants::INSTALLED_TEMPLATE_PATH.string()
  //                                +
  //                                "/" + template_hash +
  //                                "/template.json - error: " + e.what());
  //     }
  //
  //     template_file >> template_json;
  //
  //     return TemplateConfig(template_json);
  //   }
  //
  //   void InstalledTemplate::build(std::shared_ptr<Config> config) {
  //
  //     std::string hash = config->current_template.hash;
  //
  //     std::filesystem::path template_path =
  //         Constants::INSTALLED_TEMPLATE_PATH / hash;
  //
  //     std::filesystem::path override_path = config->path / "override";
  //
  //     std::unordered_map<std::string, std::filesystem::path>
  //     template_file_map;
  //
  //     if (!validate_installed(hash)) {
  //       throw TemplateNotInstalled("Template is not installed");
  //     }
  //
  //     for (auto &file : std::filesystem::recursive_directory_iterator(
  //              template_path / "template")) {
  //
  //       std::filesystem::path relative_path =
  //           std::filesystem::relative(file.path(), template_path);
  //
  //       template_file_map[relative_path.string()] = file.path();
  //     }
  //
  //     if (std::filesystem::exists(override_path)) {
  //       for (auto &file :
  //            std::filesystem::recursive_directory_iterator(override_path)) {
  //
  //         std::filesystem::path relative_path =
  //             std::filesystem::relative(file.path(), override_path);
  //
  //         template_file_map[relative_path.string()] = file.path();
  //       }
  //     }
  //
  //     for (auto [relative_path, file_path] : template_file_map) {
  //       Utils::info << "relative path: " << relative_path
  //                   << " file path: " << file_path << std::endl;
  //     }
  //
  //     exit(-1);
  //   }
  //
  //   void InstalledTemplate::refresh(std::shared_ptr<Config> config) {}
  //
  //   void to_json(nlohmann::json &json_obj,
  //                const InstalledTemplate &template_obj) {
  //     TO_JSON_FIELD(template_obj, name);
  //     TO_JSON_FIELD(template_obj, commits);
  //     TO_JSON_FIELD(template_obj, git);
  //   }
  //
  //   void from_json(const nlohmann::json &json_obj,
  //                  InstalledTemplate &template_obj) {
  //     FROM_JSON_FIELD(template_obj, name);
  //     FROM_JSON_FIELD(template_obj, commits);
  //     FROM_JSON_FIELD(template_obj, git);
  //   }
  //
  //   std::ostream &operator<<(std::ostream &os_stream,
  //                            const InstalledTemplate &template_obj) {
  //     os_stream << "Name: " << template_obj.name << std::endl;
  //     os_stream << "Commits: " << std::endl;
  //     for (System::GitCommit commit : template_obj.commits) {
  //       os_stream << commit << std::endl;
  //     }
  //     return os_stream;
  //   }
} // namespace Frate::Project
