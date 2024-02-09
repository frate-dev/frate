#include "Frate/Constants.hpp"
#include "Frate/System/GitProvider.hpp"
#include "Frate/Template/IndexEntry.hpp"
#include "Frate/Template/Renderer.hpp"
#include "Frate/Utils/CLIPrompt.hpp"
#include "Frate/Utils/General.hpp"
#include "Frate/Utils/Logging.hpp"
#include <Frate/Frate.hpp>
#include <Frate/Template/Exceptions.hpp>
#include <Frate/Template/Manager.hpp>
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
      for (TemplateIndexEntry template_json : index_json) {
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
      Utils::verbose << "Name or hash is empty" << std::endl;
      return false;
    }

    std::filesystem::path template_path =
        Constants::INSTALLED_TEMPLATE_PATH / name / hash;

    if (!installed.contains(name)) {
      Utils::verbose << "Template not installed in config, could not find key: "
                     << name << std::endl;
      return false;
    }

    Utils::verbose << nlohmann::json(installed[name]).dump(2) << std::endl;

    if (!installed[name].contains(hash)) {
      Utils::verbose << "Template not installed in config, could not find key "
                     << name << "->" << hash << std::endl;
      return false;
    }

    if (!std::filesystem::exists(template_path)) {
      Utils::verbose << "Template not installed in template path at: "
                     << template_path << std::endl;
      return false;
    }

    return true;
  }

  bool TemplateManager::is_installed(TemplateIndexEntry &entry) {
    return is_installed(entry.getName(),
                        entry.getBranchHash(Constants::TEMPLATE_BRANCH));
  }

  Project::TemplateRenderer &
  TemplateManager::find_template(const std::string &name, std::string &hash) {
    if (!is_installed(name, hash)) {
      throw TemplateNotInstalled("Template not installed");
    }
    return installed[name][hash];
  }

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

  std::vector<TemplateIndexEntry> &TemplateManager::getIndex() {
    load_index();
    return index;
  }

  std::unordered_map<
      std::string, std::unordered_map<std::string, Project::TemplateRenderer>> &
  TemplateManager::getInstalled() {
    return installed;
  }

  void TemplateManager::uninstall(std::string &name, std::string &hash) {
    if (!is_installed(name, hash)) {
      throw TemplateNotInstalled("Template not installed");
    }

    std::filesystem::path template_path =
        Constants::INSTALLED_TEMPLATE_PATH / name / hash;

    std::filesystem::remove_all(template_path);

    installed[name].erase(hash);

    if (installed[name].empty()) {
      installed.erase(name);
    }

    save();
  }

  void TemplateManager::update(std::string &name) {
    // TODO: implement
  }

  void TemplateManager::installAll() {
    load_index();
    for (TemplateIndexEntry template_info : index) {

      if (is_installed(template_info)) {
        continue;
      }

      install(template_info);
    }
  }

  void TemplateManager::deleteAll() {
    if(!std::filesystem::exists(Constants::INSTALLED_TEMPLATE_PATH)) {
      return;
    }
    std::filesystem::remove_all(Constants::INSTALLED_TEMPLATE_PATH);

    installed.clear();
    save();
  }

  void TemplateManager::updateIndex() {
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

  TemplateRenderer TemplateManager::installLatest(std::string &git_url) {
    load_index();

    for (TemplateIndexEntry template_info : index) {
      if (template_info.getGit() == git_url) {

        if (!is_installed(template_info)) {
          install(template_info);
        }

        return template_info;
      }
    }

    throw TemplateNotFoundInIndex("Template " + git_url +
                                  " not found in index");
  }

  TemplateRenderer TemplateManager::uninstallAll(std::string &name){
    if (!installed.contains(name)) {
      throw TemplateNotInstalled("Template " + name + " not installed");
    }

    std::filesystem::path template_path =
        Constants::INSTALLED_TEMPLATE_PATH / name;

    std::filesystem::remove_all(template_path);

    installed.erase(name);

    save();
  }

  TemplateRenderer TemplateManager::promptList() {

    load_index();

    std::vector<std::string> template_names;

    for (TemplateIndexEntry template_info : index) {
      template_names.push_back(template_info.getName());
    }

    auto template_prompt =
        Utils::CLI::Prompt("Template").addOptions(template_names);
    template_prompt.run();
    auto template_name = template_prompt.get<std::string>();

    for (TemplateIndexEntry template_info : index) {
      if (template_info.getName() == template_name) {

        if (!is_installed(template_info)) {
          throw TemplateNotInstalled("Template " + template_info.getName() +
                                     " not installed");
        }
        return template_info;
      }
    }
    throw TemplateNotFoundInIndex("Template " + template_name +
                                  " not found in index");
  };

  TemplateRenderer TemplateManager::get(std::string &name, std::string &hash) {

    load_index();
    for (TemplateIndexEntry template_info : index) {
      for (auto &branch : template_info.getBranches()) {
        if (template_info.getName() == name && branch == hash) {

          if (!is_installed(template_info)) {
            throw TemplateNotInstalled("Template " + template_info.getName() +
                                       " not installed");
          }

          return template_info;
        }
      }
    }
    throw TemplateNotFoundInIndex("Template " + name + " not found in index");
  }

  TemplateRenderer TemplateManager::install(TemplateIndexEntry &template_info) {
    Utils::info << "Installing template: " << template_info.getName()
                << " on branch: " << branch << std::endl;

    std::filesystem::path tmp_path =
        Utils::randomTmpPath("frate-template-download-");

    std::filesystem::path new_template_path =
        Constants::INSTALLED_TEMPLATE_PATH / template_info.getName();

    std::string selected_hash;

    if (branch.empty()) {
      selected_hash = template_info.getHead();
    }
    else {
      selected_hash = template_info.getBranchHash(branch);
    }

    new_template_path /= selected_hash;

    if (is_installed(template_info.getName(), selected_hash)) {
      Utils::warning << "Template already installed" << std::endl;
      return template_info;
    }

    System::GitProvider git(tmp_path);

    try {

      git.setBranch(branch).setRecurseSubmodules(true).clone(
          template_info.getGit());

    } catch (std::exception &e) {

      Utils::error << e.what() << std::endl;
      throw TemplateFailedToDownload("Failed to download template " +
                                     template_info.getName());
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

      template_to_installed_path(tmp_path, new_template_path, selected_hash);

    } catch (std::exception &e) {
      Utils::error << e.what() << std::endl;
      throw std::runtime_error("Failed to transfer template");
    }

    installed[template_info.getName()][selected_hash] = template_info;

    // Cleanup

    // std::filesystem::remove_all(tmp_path);

    this->save();

    return template_info;
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
      from_json(json_obj, *this);
    } catch (std::exception &e) {
      throw std::runtime_error("Failed to parse config file");
    }
  }
} // namespace Frate::Project
