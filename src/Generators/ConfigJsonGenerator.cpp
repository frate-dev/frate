#include "./Generators.hpp"
#include "../Command/Command.hpp"
#include <string>
#include <iostream>
#include <regex>
#include <termcolor/termcolor.hpp>

namespace Generators::ConfigJson{






  bool readUserInput(std::shared_ptr<Command::Context> ctx, std::shared_ptr<Config> config_json) {
    validateLang:
    if(!validateLang("📚Language->" + ctx->lang + " : ", ctx, config_json)) {
      std::cout << termcolor::red << "Invalid language - retry" << termcolor::reset << std::endl;
      goto validateLang;
    }
    validateProjectName:
    if (!validateProjectName("📖Project name->" + ctx->project_name + " : ", ctx, config_json)) {
      std::cout << termcolor::red << "Invalid project name - retry" << termcolor::reset << std::endl;
      goto validateProjectName;
    }
    validateCmakeVersion:
    if (!validateCmakeVersion("🔨Cmake version->" + ctx->cmake_version + " : ", ctx, config_json)) {
      std::cout << termcolor::red << "Invalid cmake version - retry" << termcolor::reset << std::endl;
      goto validateCmakeVersion;
    }
    validateProjectVersion: 
    if (!validateProjectVersion("🗃️Version->" + ctx->project_version + " : ", ctx, config_json)) {
      std::cout << termcolor::red << "Invalid project version - retry" << termcolor::reset << std::endl;
      goto validateProjectVersion;
    }
    validateLanguageVersion:
    if (!validateLanguageVersion("📰Language Standard->" + ctx->lang_version + " : ", ctx, config_json)) {
      std::cout << termcolor::red << "Invalid language version - retry" << termcolor::reset << std::endl;
      goto validateLanguageVersion;
    }
    validateCompiler:
    if (!validateCompiler("💽Compiler->", ctx, config_json)) {
      std::cout << termcolor::red << "Invalid compiler - retry" << termcolor::reset << std::endl;
      goto validateCompiler;
    }
    validateSourceDir:
    if (!validateSourceDir("⛲Source Dir->" + ctx->src_dir + " : ", ctx, config_json)) {
      std::cout << termcolor::red << "Invalid source directory - retry" << termcolor::reset << std::endl;
      goto validateSourceDir;
    }
    validateBuildDir:
    if (!validateBuildDir("🛠️Build Dir->" + ctx->build_dir + " : ", ctx, config_json)) {
      std::cout << termcolor::red << "Invalid build directory - retry" << termcolor::reset << std::endl;
      goto validateBuildDir;
    }
    validateIncludeDir:
    if (!validateIncludeDir("🫃Include Dir->" + ctx->include_dir + " : ", ctx, config_json)) {
      std::cout << termcolor::red << "Invalid include directory - retry" << termcolor::reset << std::endl;
      goto validateIncludeDir;
    }
    return true;
  }

  bool writeConfig(std::shared_ptr<Command::Context> &ctx) {
    std::ofstream file;
    std::string file_name = "config.json";
    file.open(ctx->project_path / file_name);
    file << ctx->toJson().dump(2);
    file << '\n';
    file.close();
    return true;
  }

} // namespace Generators::ConfigJson
