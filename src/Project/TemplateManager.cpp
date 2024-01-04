#include "Frate/Constants.hpp"
#include "Frate/System/GitProvider.hpp"
#include "Frate/TemplateMeta.hpp"
#include "Frate/Utils/General.hpp"
#include "Frate/Utils/Logging.hpp"
#include <Frate/Frate.hpp>
#include <Frate/Project/TemplateManager.hpp>
#include <Frate/Utils/Macros.hpp>
#include <filesystem>

namespace Frate {
  using Project::InstalledTemplate;

  bool TemplateManager::load_index() {
    try {
      nlohmann::json index_json =
          Utils::fetchJson(Constants::TEMPLATE_INDEX_URL);
      for (Command::TemplateMeta template_json : index_json) {
        index.push_back(template_json);
      }
      index_loaded = true;
    } catch (std::exception &e) {
      Utils::error << "Failed to load template index" << std::endl;
      return false;
    }
    return true;
  }

  bool TemplateManager::is_installed(const std::string &name,
                                     std::string &hash) {
    for (Project::InstalledTemplate template_obj : installed) {
      if (template_obj.name == name) {
        for (System::GitCommit commit : template_obj.commits) {
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
      if (template_obj.name == name) {
        return template_obj;
      }
    }
    throw std::runtime_error("Template not found");
  }

  bool TemplateManager::template_to_installed_path(
      std::filesystem::path &tmp_path, std::filesystem::path &new_template_path,
      std::string &hash) {

    Utils::verbose << "Transferring template from" << tmp_path << " to "
                   << new_template_path << std::endl;

    for (std::filesystem::directory_entry entry :
         std::filesystem::recursive_directory_iterator(tmp_path)) {
      std::string relative_path =
          entry.path().string().substr(tmp_path.string().length() + 1);

      // We don't want to copy the .git folder
      if (relative_path.find(".git") != std::string::npos) {
        continue;
      }

      std::filesystem::path new_file_path = new_template_path / relative_path;

      Utils::verbose << "Copying: " << relative_path + " to "
                     << new_file_path.string() << std::endl;

      try {

        if (std::filesystem::is_directory(entry)) {
          std::filesystem::create_directories(new_file_path);
          continue;
        }

      } catch (std::exception &e) {

        Utils::error << "Failed to create directories at: " << new_template_path
                     << std::endl;
        return false;
      }

      try {

        std::filesystem::copy_file(entry.path(), new_file_path);

      } catch (std::exception &e) {

        Utils::error << "Failed to copy file: " << relative_path << std::endl;
        return false;
      }
    }

    return true;
  }

  std::vector<Command::TemplateMeta> &TemplateManager::getIndex() {
    if (index.empty() && !index_loaded) {
      if (!load_index()) {
        exit(-1);
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

  Command::TemplateMeta TemplateManager::install(const std::string &name,
                                                 std::string hash) {
    Utils::info << "Installing template: " << name << std::endl;
    if (!load_index()) {
      throw std::runtime_error("Failed to load template index");
    }

    std::filesystem::path tmp_path =
        Utils::randomTmpPath("frate-template-download-");

    std::filesystem::path new_template_path =
        Constants::INSTALLED_TEMPLATE_PATH / name;

    System::GitProvider git(tmp_path);
    git.setBranch("new_function_prefix");

    for (Command::TemplateMeta template_info : index) {
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

        if (!template_to_installed_path(tmp_path, new_template_path,
                                        latest_commit.hash)) {

          throw std::runtime_error("Failed to transfer template");
        }

        InstalledTemplate installed_template;
        installed_template.name = name;
        installed_template.git = template_info.git;
        // We only want to add the latest commit
        installed_template.commits.push_back(latest_commit);

        Utils::info << "Putting template in config: " << installed_template
                    << std::endl;

        // Installing the template in the config
        installed.push_back(installed_template);

        // Cleanup

        // std::filesystem::remove_all(tmp_path);

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

} // namespace Frate
