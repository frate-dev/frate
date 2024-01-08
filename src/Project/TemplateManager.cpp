#include "Frate/Constants.hpp"
#include "Frate/Project/TemplateMeta.hpp"
#include "Frate/System/GitProvider.hpp"
#include "Frate/Utils/General.hpp"
#include "Frate/Utils/Logging.hpp"
#include <Frate/Frate.hpp>
#include <Frate/Project/TemplateManager.hpp>
#include <Frate/Utils/FileFilter.hpp>
#include <Frate/Utils/Macros.hpp>
#include <filesystem>
#include <fstream>

namespace Frate::Project {
  using Project::InstalledTemplate;

  void TemplateManager::load_index() {
    try {
      nlohmann::json index_json =
          Utils::fetchJson(Constants::TEMPLATE_INDEX_URL);
      for (TemplateMeta template_json : index_json) {
        index.push_back(template_json);
      }
      index_loaded = true;
    } catch (std::exception &e) {
      Utils::error << "Failed to load template index" << std::endl;
      Utils::error << e.what() << std::endl;
      throw std::runtime_error("Failed to load template index");
    }
  }

  bool TemplateManager::is_installed(const std::string &name,
                                     std::string &hash) {

    if (name.empty() || hash.empty()) {
      Utils::error << "Name or hash is empty" << std::endl;
      return false;
    }

    for (Project::InstalledTemplate template_obj : installed) {
      if (template_obj.getName() == name) {
        for (System::GitCommit commit : template_obj.getCommits()) {
          if (commit.hash == hash) {
            return true;
          }
        }
      }
    }
    return false;
  }

  Project::InstalledTemplate &
  TemplateManager::find_template(const std::string &name) {
    for (Project::InstalledTemplate &template_obj : installed) {
      if (template_obj.getName() == name) {
        return template_obj;
      }
    }
    throw std::runtime_error("Template not found");
  }

  void TemplateManager::template_to_installed_path(
      std::filesystem::path &tmp_path, std::filesystem::path &new_template_path,
      std::string &hash) {

    Utils::verbose << "Transferring template from" << tmp_path << " to "
                   << new_template_path << std::endl;
    Utils::FileFilter filter(tmp_path);
    filter.addPaths({
        ".git",
        ".gitignore",
        ".gitmodules",
        ".gitattributes",
    });

    std::vector<std::filesystem::path> files_to_copy =
        filter.filterOut(tmp_path);

    for (std::filesystem::path tmp_filtered_path : files_to_copy) {

      std::filesystem::path new_file_path =
          new_template_path /
          std::filesystem::relative(tmp_filtered_path, tmp_path);

      Utils::verbose << "Copying: " << tmp_filtered_path << " to "
                     << new_file_path << std::endl;

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
    if (index.empty() && !index_loaded) {
      try {
        load_index();
      } catch (std::exception &e) {
        Utils::error << e.what() << std::endl;
        throw std::runtime_error("Failed to load template index");
      }
    }
    return index;
  }

  std::filesystem::path
  TemplateManager::makeTemplate(const std::filesystem::path &override_path,
                                const std::string &name, std::string &hash) {

    std::filesystem::path tmp_gen_path =
        Utils::randomTmpPath("frate-template-render-");

    std::filesystem::path template_path =
        Constants::INSTALLED_TEMPLATE_PATH / name / hash;

    InstalledTemplate installed_temp;

    try {
      installed_temp = find_template(name);
    } catch (std::exception &e) {
      Utils::warning << "Couldn't find template with name: " << name
                     << " and hash: " << hash << std::endl;
      Utils::warning << "Installing template" << std::endl;
      try {
        install(name, hash);
      } catch (std::exception &e) {
        Utils::error << "Failed to install template" << std::endl;
        Utils::error << e.what() << std::endl;
        throw std::runtime_error("Failed to install template");
      }
    }

    if (installed.empty()) {
      throw std::runtime_error("Installed templates is empty");
    }

    try {
      installed_temp = find_template(name);
    } catch (std::exception &e) {
      Utils::error << e.what() << std::endl;
      throw std::runtime_error("Failed to find template after failing to "
                               "install it the second time");
    }

    if (!std::filesystem::exists(template_path)) {
      throw std::runtime_error("Template not found in path");
    }

    Utils::verbose << "Copying template from: " << template_path << " to "
                   << tmp_gen_path << std::endl;

    for (std::filesystem::directory_entry entry :
         std::filesystem::recursive_directory_iterator(template_path)) {
      std::string relative_path =
          entry.path().string().substr(template_path.string().length() + 1);

      std::filesystem::path new_file_path = tmp_gen_path / relative_path;

      Utils::verbose << "Copying: " << relative_path + " to "
                     << new_file_path.string() << std::endl;

      try {

        if (std::filesystem::is_directory(entry)) {
          std::filesystem::create_directories(new_file_path);
          continue;
        }

      } catch (std::exception &e) {

        throw std::runtime_error("Failed to create directories: " +
                                 tmp_gen_path.string());
      }

      try {

        std::filesystem::copy_file(entry.path(), new_file_path);

      } catch (std::exception &e) {
        throw std::runtime_error("Failed to copy file: " + relative_path);
      }
    }

    if (std::filesystem::exists(override_path)) {
      Utils::verbose << "Copying override files from: " << override_path
                     << " to " << tmp_gen_path << std::endl;

      for (std::filesystem::directory_entry entry :
           std::filesystem::recursive_directory_iterator(override_path)) {
        std::string relative_path =
            entry.path().string().substr(override_path.string().length() + 1);

        std::filesystem::path new_file_path = tmp_gen_path / relative_path;

        Utils::verbose << "Copying: " << relative_path + " to "
                       << new_file_path.string() << std::endl;
      }
    }
    return tmp_gen_path;
  }

  std::vector<Project::InstalledTemplate> &TemplateManager::getInstalled() {
    return installed;
  }

  TemplateMeta TemplateManager::install(const std::string &name,
                                        std::string hash) {
    Utils::info << "Installing template: " << name << std::endl;

    try {
      load_index();
    } catch (std::exception &e) {
      Utils::error << e.what() << std::endl;
      throw std::runtime_error("Failed to load template index");
    }

    std::filesystem::path tmp_path =
        Utils::randomTmpPath("frate-template-download-");

    std::filesystem::path new_template_path =
        Constants::INSTALLED_TEMPLATE_PATH / name;

    System::GitProvider git(tmp_path);
    git.setBranch("new_function_prefix");

    for (TemplateMeta template_info : index) {
      // Searching for the template in index
      if (template_info.name == name) {
        Utils::info << "Downloading template at: " << template_info.git
                    << std::endl;

        try {

          git.setNoCheckout(true).clone(template_info.git).log();

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

        template_info.hash = latest_commit.hash;

        if (!hash.empty()) {
          template_info.hash = hash;
        }
        if (is_installed(name, template_info.hash)) {
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

          throw std::runtime_error("Failed to create directories: " +
                                   new_template_path.string());
        }

        try {

          template_to_installed_path(tmp_path, new_template_path,
                                     latest_commit.hash);

        } catch (std::exception &e) {

          throw std::runtime_error("Failed to transfer template");
        }

        InstalledTemplate installed_template;
        installed_template.getName() = name;
        installed_template.getGit() = template_info.git;
        // We only want to add the latest commit
        installed_template.getCommits().push_back(latest_commit);

        Utils::info << "Putting template in config: " << installed_template
                    << std::endl;

        // Installing the template in the config
        installed.push_back(installed_template);

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
