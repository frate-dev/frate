#include "Frate/Constants.hpp"
#include "Frate/Project/TemplateMeta.hpp"
#include "Frate/System/GitProvider.hpp"
#include "Frate/Utils/General.hpp"
#include "Frate/Utils/Logging.hpp"
#include <Frate/Frate.hpp>
#include <Frate/Project/Exceptions.hpp>
#include <Frate/Project/TemplateManager.hpp>
#include <Frate/Utils/FileFilter.hpp>
#include <Frate/Utils/Macros.hpp>
#include <filesystem>
#include <fstream>

namespace Frate::Project {

  void TemplateManager::load_index() {

    if (this->index_loaded) {
      return;
    }

    Utils::verbose << "Attempting to load template index file" << std::endl;

    if (!std::filesystem::exists(Constants::TEMPLATE_INDEX_PATH)) {
      Utils::verbose << "Template index file doesn't exist" << std::endl;
    }
    else {
      std::ifstream file(Constants::TEMPLATE_INDEX_PATH);
      if (!file.is_open()) {
        Utils::error << "Failed to open template index file" << std::endl;
        throw TemplateIndexNotLoaded("Failed to open template index file");
      }

      nlohmann::json json_obj;

      try {
        file >> json_obj;
      } catch (std::exception &e) {
        throw TemplateIndexFailedToLoad("Failed to parse template index file");
      }

      this->index = json_obj;
      this->index_loaded = true;
      Utils::verbose << "Loaded template index file" << std::endl;

      return;
    }

    Utils::verbose << "Attempting to load template index upstream" << std::endl;
    try {
      nlohmann::json index_json =
          Utils::fetchJson(Constants::TEMPLATE_INDEX_URL);
      for (TemplateMeta template_json : index_json) {
        index.push_back(template_json);
      }
      index_loaded = true;

      try {
        std::ofstream file(Constants::TEMPLATE_INDEX_PATH);
        if (!file.is_open()) {
          Utils::error
              << "Failed to open template index file to save upstream index"
              << std::endl;
        }
        else {
          file << index_json.dump(2);
          file.close();
        }
      } catch (std::exception &e) {
        Utils::error << "Failed to save template upstream index file"
                     << std::endl;
      }

    } catch (std::exception &e) {
      Utils::error << "Failed to load template index" << std::endl;
      Utils::error << e.what() << std::endl;
      throw TemplateIndexNotLoaded("Failed to load template index");
    }
  }

  bool TemplateManager::is_installed(const std::string &name,
                                     std::string &hash) {

    if (name.empty() || hash.empty()) {
      Utils::error << "Name or hash is empty" << std::endl;
      return false;
    }

    std::filesystem::path template_path =
        Constants::INSTALLED_TEMPLATE_PATH / name / hash;

    if (!installed.contains(name)) {
      Utils::error << "Template not installed in config" << std::endl;
      return false;
    }

    if (!installed[name].contains(hash)) {
      Utils::error << "Template not installed in config" << std::endl;
      return false;
    }

    if (!std::filesystem::exists(template_path)) {
      Utils::error << "Template not installed in template path at: "
                   << template_path << std::endl;
      return false;
    }

    return true;
  }

  Project::TemplateMeta &TemplateManager::find_template(const std::string &name,
                                                        std::string &hash) {
    if (!is_installed(name, hash)) {
      throw TemplateNotInstalled("Template not installed");
    }
    return installed[name][hash];
  }

  std::string TemplateManager::get_latest_hash(const std::string &name){
    //TODO: implement
    //BLOCKED: need index template class
//     Utils::verbose << "Getting latest hash for template: " << name << std::endl;
// 
//     TemplateManager templ;
//     templ.load_index();
// 
//     for (TemplateMeta template_info : templ.getIndex()) {
//       if (template_info.getName() == name) {
//         return template_info.getHash();
//       }
//     }
//     throw std::runtime_error("Template not found");

  };
  void TemplateManager::template_to_installed_path(
      std::filesystem::path &tmp_path, std::filesystem::path &new_template_path,
      std::string &hash) {

    Utils::verbose << "Transferring template from" << tmp_path << " to "
                   << new_template_path << std::endl;
    Utils::FileFilter filter(tmp_path);
    filter.addDirs({
        ".git",
        ".gitignore",
        ".gitmodules",
        ".gitattributes",
    });

    std::vector<std::filesystem::path> files_to_copy = filter.filterOut();

    for (std::filesystem::path tmp_filtered_path : files_to_copy) {

      std::filesystem::path new_file_path =
          new_template_path /
          std::filesystem::relative(tmp_filtered_path, tmp_path);

      Utils::verbose << "Copying: " << tmp_filtered_path << " to "
                     << new_file_path << std::endl;

      if (!std::filesystem::exists(new_file_path.parent_path())) {
        std::filesystem::create_directories(new_file_path.parent_path());
      }

      try {
        std::filesystem::copy(
            tmp_filtered_path, new_file_path,
            std::filesystem::copy_options::recursive |
                std::filesystem::copy_options::overwrite_existing);
      } catch (std::exception &e) {
        Utils::error << e.what() << std::endl;
        throw std::runtime_error("Failed to copy file");
      }
    }
  }

  std::vector<Project::TemplateMeta> &TemplateManager::getIndex() {
    load_index();
    return index;
  }

  std::unordered_map<std::string,
                     std::unordered_map<std::string, Project::TemplateMeta>> &
  TemplateManager::getInstalled() {
    return installed;
  }

  void updateIndex() {
    Utils::verbose << "Updating template index" << std::endl;

    if (!std::filesystem::exists(Constants::TEMPLATE_INDEX_PATH)) {
      Utils::verbose << "Template index file doesn't exist" << std::endl;

      std::ofstream file(Constants::TEMPLATE_INDEX_PATH);

      if (!file.is_open()) {
        Utils::error << "Failed to open template index file" << std::endl;
        throw std::runtime_error("Failed to open template index file");
      }

      file.close();
    }

    try {
      auto upstream_index = Utils::fetchJson(Constants::TEMPLATE_INDEX_URL);
      std::ofstream file(Constants::TEMPLATE_INDEX_PATH);
      if (!file.is_open()) {
        Utils::error
            << "Failed to open template index file to save upstream index"
            << std::endl;
      }
      else {
        file << upstream_index.dump(2);
        file.close();
      }
    } catch (std::exception &e) {
      Utils::error << "Failed to save template upstream index file"
                   << std::endl;
    }
  }

  TemplateMeta TemplateManager::install(const std::string &name,
                                        std::string hash) {
    Utils::info << "Installing template: " << name << std::endl;

    load_index();

    std::filesystem::path tmp_path =
        Utils::randomTmpPath("frate-template-download-");

    std::filesystem::path new_template_path =
        Constants::INSTALLED_TEMPLATE_PATH / name;

    System::GitProvider git(tmp_path);
    git.setBranch("new_function_prefix");

    for (TemplateMeta template_info : index) {
      // Searching for the template in index
      if (template_info.getName() == name) {
        Utils::info << "Downloading template at: " << template_info.getGit()
                    << std::endl;

        try {

          git.setNoCheckout(true).clone(template_info.getGit()).log();

        } catch (std::exception &e) {
          Utils::error << e.what() << std::endl;
          throw std::runtime_error("Failed to clone template " + name);
        }

        if (git.getCommits().empty()) {
          throw std::runtime_error("Failed to get commits");
        }

        System::GitCommit latest_commit = git.getCommits().front();

        Utils::info << "Latest commit: " << latest_commit << std::endl;

        // appending on the hash to the path so we can have multiple versions
        new_template_path /= latest_commit.hash;

        template_info.setHash(latest_commit.hash);

        if (!hash.empty()) {
          template_info.setHash(hash);
        }
        if (is_installed(name, template_info.getHash())) {
          Utils::warning << "Template already installed" << std::endl;
          return template_info;
        }

        try {

          git.setRecurseSubmodules(true)
              .checkout(hash)
              .pull()
              .setInit(true)
              .setRemote(true)
              .submoduleUpdate();

        } catch (std::exception &e) {

          Utils::error << e.what() << std::endl;
          throw std::runtime_error("Failed to checkout template");
        }

        if (std::filesystem::exists(new_template_path)) {
          Utils::verbose << "Template already exists at: " << new_template_path
                         << " we're going to delete it because we already "
                            "assume that it's not installed"
                         << std::endl;
          // This should only happen in rare cases
          std::filesystem::remove_all(new_template_path);
        }

        try {

          std::filesystem::create_directories(new_template_path);

        } catch (std::exception &e) {

          Utils::error << e.what() << std::endl;
          throw std::runtime_error("Failed to create directories: " +
                                   new_template_path.string());
        }

        try {

          template_to_installed_path(tmp_path, new_template_path,
                                     latest_commit.hash);

        } catch (std::exception &e) {
          Utils::error << e.what() << std::endl;
          throw std::runtime_error("Failed to transfer template");
        }

        installed[name][latest_commit.hash] = template_info;

        // Cleanup

        std::filesystem::remove_all(tmp_path);

        return template_info;
      }
    }
    throw std::runtime_error("Template not found");
  }

  void from_json(const nlohmann::json &json_obj, TemplateManager &templ) {
    FROM_JSON_FIELD(templ, installed);
  }

  void to_json(nlohmann::json &json_obj, const TemplateManager &templ) {
    TO_JSON_FIELD(templ, installed);
  }

  void TemplateManager::save() {
    Utils::verbose << "Saving template manager" << std::endl;

    std::filesystem::path config_dir =
        Constants::INSTALLED_TEMPLATE_CONFIG_PATH.parent_path();

    std::filesystem::path config_path =
        Constants::INSTALLED_TEMPLATE_CONFIG_PATH;

    if (!std::filesystem::exists(config_dir)) {
      Utils::verbose << "Creating config directory: " << config_dir.string()
                     << std::endl;
      std::filesystem::create_directories(config_dir);
    }

    std::ofstream file(config_path);

    if (!file.is_open()) {
      Utils::error << "Failed to open config file: " << config_path
                   << std::endl;
      throw std::runtime_error("Failed to open config file");
    }

    nlohmann::json json_obj = *this;

    file << json_obj.dump(2);
    file.close();
  }

  void TemplateManager::load() {
    Utils::verbose << "Loading template manager" << std::endl;

    std::filesystem::path config_path =
        Constants::INSTALLED_TEMPLATE_CONFIG_PATH;

    if (!std::filesystem::exists(config_path)) {
      throw std::runtime_error("Config file doesn't exist");
    }

    std::ifstream file(config_path);

    if (!file.is_open()) {
      throw std::runtime_error("Failed to open config file");
    }

    nlohmann::json json_obj;

    try {
      file >> json_obj;
    } catch (std::exception &e) {
      throw std::runtime_error("Failed to parse config file");
    }

    try {
      *this = json_obj;
    } catch (std::exception &e) {
      throw std::runtime_error("Failed to parse config file");
    }
  }
} // namespace Frate::Project
