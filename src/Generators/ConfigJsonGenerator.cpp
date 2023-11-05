#include "./Generators.hpp"
#include "../Command/Command.hpp"
#include <string>
#include <iostream>
#include <regex>
#include <termcolor/termcolor.hpp>

namespace Generators::ConfigJson{






  bool readUserInput(std::shared_ptr<Command::Project> pro, std::shared_ptr<Config> config_json) {
    validateLang:
    if(!validateLang("📚Language->" + pro->lang + " : ", pro, config_json)) {
      std::cout << termcolor::red << "Invalid language - retry" << termcolor::reset << std::endl;
      goto validateLang;
    }
    validateProjectName:
    if (!validateProjectName("📖Project name->" + pro->project_name + " : ", pro, config_json)) {
      std::cout << termcolor::red << "Invalid project name - retry" << termcolor::reset << std::endl;
      goto validateProjectName;
    }
    validateCmakeVersion:
    if (!validateCmakeVersion("🔨Cmake version->" + pro->cmake_version + " : ", pro, config_json)) {
      std::cout << termcolor::red << "Invalid cmake version - retry" << termcolor::reset << std::endl;
      goto validateCmakeVersion;
    }
    validateProjectVersion: 
    if (!validateProjectVersion("🗃️Version->" + pro->project_version + " : ", pro, config_json)) {
      std::cout << termcolor::red << "Invalid project version - retry" << termcolor::reset << std::endl;
      goto validateProjectVersion;
    }
    validateLanguageVersion:
    if (!validateLanguageVersion("📰Language Standard->" + pro->lang_version + " : ", pro, config_json)) {
      std::cout << termcolor::red << "Invalid language version - retry" << termcolor::reset << std::endl;
      goto validateLanguageVersion;
    }
    validateCompiler:
    if (!validateCompiler("💽Compiler->", pro, config_json)) {
      std::cout << termcolor::red << "Invalid compiler - retry" << termcolor::reset << std::endl;
      goto validateCompiler;
    }
    validateSourceDir:
    if (!validateSourceDir("⛲Source Dir->" + pro->src_dir + " : ", pro, config_json)) {
      std::cout << termcolor::red << "Invalid source directory - retry" << termcolor::reset << std::endl;
      goto validateSourceDir;
    }
    validateBuildDir:
    if (!validateBuildDir("🛠️Build Dir->" + pro->build_dir + " : ", pro, config_json)) {
      std::cout << termcolor::red << "Invalid build directory - retry" << termcolor::reset << std::endl;
      goto validateBuildDir;
    }
    validateIncludeDir:
    if (!validateIncludeDir("🫃Include Dir->" + pro->include_dir + " : ", pro, config_json)) {
      std::cout << termcolor::red << "Invalid include directory - retry" << termcolor::reset << std::endl;
      goto validateIncludeDir;
    }
    return true;
  }

  bool writeConfig(std::shared_ptr<Command::Project> &pro) {
    std::ofstream file;
    std::string file_name = "config.json";
    file.open(pro->project_path / file_name);
    file << pro->toJson();
    file << '\n';
    file.close();
    return true;
  }

} // namespace Generators::ConfigJson
