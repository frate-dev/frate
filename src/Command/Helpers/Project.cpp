#include "Frate/Utils/General.hpp"
#include <Frate/Generators.hpp>
#include <Frate/Project.hpp>
#include <Frate/ProjectPrompt.hpp>
#include <Frate/Template.hpp>
#include <Frate/Utils/Macros.hpp>
#include <fstream>
#include <nlohmann/json_fwd.hpp>

namespace Frate::Command {

  Project::Project() = default;

  bool Project::save() {
    std::ofstream file;
    std::string file_name = "frate-project.json";
    // Used as a safe guard to prevent overwriting the project file
    if (!loaded_json) {
      Utils::error << "Project not loaded" << std::endl;
      return false;
    }

    if (!std::filesystem::exists(this->path)) {
      try {
        std::filesystem::create_directory(this->path);
      } catch (std::exception &e) {
        Utils::debug(e.what());
        Utils::error << "Error while creating directory: " << this->path
                     << std::endl;
        return false;
      }
    }

    if (!std::filesystem::exists(this->path / file_name)) {
      std::cout << "Creating new project file" << std::endl;
    }
    json new_json = *this;

    Utils::info << "Writing to file: " << this->path / file_name << std::endl;

    try {
      file.open(this->path / file_name);
    } catch (std::exception &e) {
      Utils::debug(e.what());
      return false;
    }

    file << new_json.dump(2);

    // Generators::CMakeList::createCMakeLists(std::make_shared<Project>(*this));
    return true;
  }

  std::string Project::safePath() {
    return "'" + std::filesystem::path(this->path).string() + "'";
  };

  void Project::fromTemplate(Template &temp) {
    this->src_dir = temp.src_dir;
    this->include_dir = temp.include_dir;
    this->build_dir = temp.build_dir;
    this->dependencies = temp.dependencies;
    this->default_mode = temp.default_mode;
    this->modes = temp.modes;
    this->flags = temp.flags;
    this->global = temp.global;
    this->prompts = temp.prompts;
    this->libs = temp.system_libs;
  }

  void from_json(const json &json_obj, Project &pro) {

    FROM_JSON_FIELD(pro, name);
    FROM_JSON_FIELD(pro, description);
    FROM_JSON_FIELD(pro, current_template);
    FROM_JSON_FIELD(pro, cmake_version);
    FROM_JSON_FIELD(pro, version);
    FROM_JSON_FIELD(pro, type);
    FROM_JSON_FIELD(pro, lang);
    FROM_JSON_FIELD(pro, lang_version);
    FROM_JSON_FIELD(pro, compiler);
    FROM_JSON_FIELD(pro, src_dir);
    FROM_JSON_FIELD(pro, build_dir);
    FROM_JSON_FIELD(pro, include_dir);
    FROM_JSON_FIELD(pro, authors);
    FROM_JSON_FIELD(pro, default_mode);
    FROM_JSON_FIELD(pro, keywords);
    FROM_JSON_FIELD(pro, libs);
    FROM_JSON_FIELD(pro, dependencies);
    FROM_JSON_FIELD(pro, flags);
    FROM_JSON_FIELD(pro, toolchains);
    FROM_JSON_FIELD(pro, prompts);
    FROM_JSON_FIELD(pro, global);
    FROM_JSON_FIELD(pro, modes);
    FROM_JSON_FIELD(pro, git);
    FROM_JSON_FIELD(pro, homepage);
    FROM_JSON_FIELD(pro, bugs);
  }

  void to_json(json &json_obj, const Project &pro) {
    TO_JSON_FIELD(pro, name);
    TO_JSON_FIELD(pro, description);
    TO_JSON_FIELD(pro, current_template);
    TO_JSON_FIELD(pro, cmake_version);
    TO_JSON_FIELD(pro, version);
    TO_JSON_FIELD(pro, type);
    TO_JSON_FIELD(pro, lang);
    TO_JSON_FIELD(pro, lang_version);
    TO_JSON_FIELD(pro, compiler);
    TO_JSON_FIELD(pro, src_dir);
    TO_JSON_FIELD(pro, build_dir);
    TO_JSON_FIELD(pro, include_dir);
    TO_JSON_FIELD(pro, authors);
    TO_JSON_FIELD(pro, default_mode);
    TO_JSON_FIELD(pro, keywords);
    TO_JSON_FIELD(pro, libs);
    TO_JSON_FIELD(pro, dependencies);
    TO_JSON_FIELD(pro, flags);
    TO_JSON_FIELD(pro, toolchains);
    TO_JSON_FIELD(pro, prompts);
    TO_JSON_FIELD(pro, global);
    TO_JSON_FIELD(pro, modes);
    TO_JSON_FIELD(pro, git);
    TO_JSON_FIELD(pro, homepage);
    TO_JSON_FIELD(pro, bugs);
  }

  bool Project::load() {
    using nlohmann::json;
    std::fstream file;
    std::string file_name = "frate-project.json";

    Utils::info << "Loading: " << (this->path / file_name) << std::endl;

    if (!std::filesystem::exists(this->path / file_name)) {
      return false;
    }

    try {
      file.open(this->path / file_name);
      std::filesystem::path current_path = this->path;
      json j = json::parse(file);
      Project temp = j;
      *this = temp;
      this->path = current_path;
    } catch (std::exception &e) {
      Utils::debug(e.what());
      return false;
    }
    // Used to prevent overwriting the project file
    loaded_json = true;
    return true;
  }
} // namespace Frate::Command
