#include "Frate/Constants.hpp"
#include "Frate/Lua/TemplateEnvironment.hpp"
#include <Frate/Project/Config.hpp>
#include <Frate/Project/Exceptions.hpp>
#include <Frate/Project/TemplateMeta.hpp>
#include <Frate/Utils/FileFilter.hpp>
#include <Frate/Utils/Logging.hpp>
#include <Frate/Utils/Macros.hpp>
#include <filesystem>
#include <fstream>
#include <sol/stack.hpp>

namespace Frate::Project {
  using std::filesystem::directory_entry;

  TemplateMeta::TemplateMeta(const nlohmann::json &json_obj): install_path(Constants::INSTALLED_TEMPLATE_PATH / this->name / this->hash) {
    Utils::verbose << "Creating template meta from json with contents: "
                   << json_obj << std::endl;

    from_json(json_obj, *this);
    file_map = {};
  }

  TemplateMeta::TemplateMeta(): install_path(Constants::INSTALLED_TEMPLATE_PATH / this->name / this->hash) {
    file_map = {};
  }

  TemplateMeta::TemplateMeta(TemplateIndexEntry &entry)
      : name(entry.getName()), description(entry.getDescription()),
        hash(entry.getLatestHash()), git(entry.getGit()),
        install_path(Constants::INSTALLED_TEMPLATE_PATH / this->name /
                     this->hash) {}

  void from_json(const nlohmann::json &json_obj, TemplateMeta &temp) {
    FROM_JSON_FIELD(temp, name);
    FROM_JSON_FIELD(temp, description);
    FROM_JSON_FIELD(temp, hash);
    FROM_JSON_FIELD(temp, git);
  }

  void to_json(nlohmann::json &json_obj, const TemplateMeta &temp) {
    TO_JSON_FIELD(temp, name);
    TO_JSON_FIELD(temp, description);
    TO_JSON_FIELD(temp, hash);
    TO_JSON_FIELD(temp, git);
  }

  std::ostream &operator<<(std::ostream &os_stream, const TemplateMeta &temp) {
    os_stream << "Name: " << temp.name << std::endl;
    os_stream << "Description: " << temp.description << std::endl;
    os_stream << "Hash: " << temp.hash << std::endl;
    os_stream << "Git: " << temp.git << std::endl;
    return os_stream;
  }

  void TemplateMeta::build(std::shared_ptr<Config> config) {

    Utils::verbose << "Building template from template at: " << install_path
                   << std::endl;

    std::filesystem::path template_config_path = install_path / "template.json";

    if (!std::filesystem::exists(install_path)) {
      throw TemplateNotInstalled("Template not installed in path");
    }

    if (!std::filesystem::exists(template_config_path)) {
      throw TemplateConfigNotFound("Template config not found");
    }

    for (const directory_entry &file :
         std::filesystem::recursive_directory_iterator(install_path)) {

      file_map[std::filesystem::relative(file.path(), install_path)] =
          file.path();
    }

    Utils::verbose << "Opening template config: " << template_config_path
                   << std::endl;

    std::ifstream template_config_file;
    try {

      template_config_file.open(template_config_path);

    } catch (std::exception &e) {

      Utils::error << "Failed to open template config: " << template_config_path
                   << std::endl;

      Utils::error << "Error: " << e.what() << std::endl;
    }

    nlohmann::json template_config_json;

    template_config_file >> template_config_json;

    template_config_file.close();

    TemplateConfig template_config = template_config_json;

    config->fromTemplate(template_config);

    Utils::info << "Template built" << std::endl;
    install_cpm(config);
    render(config);
  }

  void TemplateMeta::refresh(std::shared_ptr<Config> config) {

    std::filesystem::path override_path = config->path / "override";

    Utils::FileFilter template_filter(install_path);
    template_filter.addDirs(
        {"scripts", "__init__", "__post__", "cmake_includes"});
    template_filter.addFiles({"CMakeLists.txt"});

    auto install_path_files = template_filter.filterIn();

    for (auto &file : install_path_files) {
      std::filesystem::path relative_path =
          std::filesystem::relative(file, install_path);
      file_map[relative_path.string()] = file;
    }

    Utils::FileFilter override_filter(override_path);
    override_filter.addDirs(
        {"scripts", "__init__", "__post__", "cmake_includes"});
    override_filter.addFiles({"CMakeLists.txt"});

    auto override_path_files = override_filter.filterIn();

    for (auto &file : override_path_files) {
      std::filesystem::path relative_path =
          std::filesystem::relative(file, override_path);
      file_map[relative_path.string()] = file;
    }

    render(config);
  }

  void TemplateMeta::load_scripts() {
    if (file_map.empty()) {
      throw TemplateFileMapEmpty("File map is empty");
    }

    const std::string script_key = "scripts.";
    const std::string init_key = "__init__.";
    const std::string post_key = "__post__.";

    for (auto [relative_path, file_path] : file_map) {

      if (file_path.extension() == ".lua") {
        std::string namespace_str =
            Lua::TemplateEnvironment::relativePathToNamespace(relative_path);

        if (namespace_str.empty()) {
          throw Lua::LuaException("Namespace string is empty");
        }

        std::ifstream script_file;
        std::string script_text;
        try {
          script_file.open(file_path);
          while (script_file.good()) {
            std::string line;
            std::getline(script_file, line);
            script_text += line + "\n";
          }
        } catch (std::exception &e) {
          Utils::error << "Error reading script file: " << file_path
                       << std::endl;
          Utils::error << "Error: " << e.what() << std::endl;
        }

        if (namespace_str.starts_with("scripts")) {
          namespace_str.erase(0, script_key.length());
          Utils::verbose << "Registering script: " << namespace_str
                         << std::endl;
          env->registerMacroScript(namespace_str, script_text);
        }
        else if (namespace_str.starts_with("__init__")) {
          namespace_str.erase(0, init_key.length());
          Utils::verbose << "Registering init script: " << namespace_str
                         << std::endl;
          env->registerInitScript(namespace_str, script_text);
        }
        else if (namespace_str.starts_with("__post__")) {
          namespace_str.erase(0, post_key.length());
          Utils::verbose << "Registering post script: " << namespace_str
                         << std::endl;
          env->registerPostScript(namespace_str, script_text);
        }
        else {
          Utils::verbose << "Registering script: " << namespace_str
                         << std::endl;
          env->registerMacroScript(namespace_str, script_text);
        }
      }
    }
    scripts_loaded = true;
  }

  void TemplateMeta::install_cpm(std::shared_ptr<Config> config) {
    std::string cpm;

    cpm = Utils::fetchText("https://raw.githubusercontent.com/cpm-cmake/"
                           "CPM.cmake/v0.38.6/cmake/CPM.cmake");

    std::ofstream cpm_file;
    try {
      if (!std::filesystem::exists(config->path / "cmake")) {
        std::filesystem::create_directories(config->path / "cmake");
      }
      cpm_file.open(config->path / "cmake/CPM.cmake");
    } catch (...) {
      Utils::error << "Error while opening file: CPM.cmake" << std::endl;
      throw Lua::LuaException("Error while opening file: CPM.cmake");
    }

    cpm_file << cpm;
  }

  void TemplateMeta::render(std::shared_ptr<Config> config) {
    Utils::info << "Rendering template" << std::endl;
    this->env = std::make_shared<Lua::TemplateEnvironment>(config);

    if (!scripts_loaded) {

      try {
        load_scripts();
      } catch (std::exception &e) {
        Utils::error << "Error loading scripts: " << e.what() << std::endl;
        throw Lua::LuaException("Error loading scripts");
      }
    }

    // Generate a list of all files that are not templates
    std::vector<std::filesystem::path> all_files;

    for (auto [relative_path, file_path] : file_map) {
      all_files.emplace_back(file_path);
    }

    Utils::FileFilter non_template_filter(all_files);

    non_template_filter.addExtensions({".lua", ".inja"});
    non_template_filter.addDirs(
        {"scripts", "__init__", "__post__", "cmake_includes"});
    non_template_filter.addFiles({"template.json"});

    all_files = non_template_filter.filterOut();

    for (auto &file : all_files) {
      // Finds the relative path i nthe original file map and copies it to the
      // output directory
      for (auto [relative_path, file_path] : file_map) {
        if (file_path == file) {
          std::filesystem::path output_file = config->path / relative_path;
          Utils::verbose << "Copying file: " << file_path << " to "
                         << output_file << std::endl;
          if (!std::filesystem::exists(output_file.parent_path())) {
            std::filesystem::create_directories(output_file.parent_path());
          }
          std::filesystem::copy(
              file_path, output_file,
              std::filesystem::copy_options::recursive |
                  std::filesystem::copy_options::overwrite_existing);
        }
      }
    }

    for (auto [relative_path, file_path] : file_map) {

      if (file_path.extension() == ".inja") {
        Utils::verbose << "Rendering template: " << file_path << std::endl;
        std::string output_file = config->path / relative_path;

        output_file = output_file.substr(0, output_file.find(".inja"));

        if (env->getProjectConfig() == nullptr) {
          throw Lua::LuaException(
              "Project config is null before templating file");
        }

        env->templateFile(file_path, output_file);
      }
    }
  }

} // namespace Frate::Project
