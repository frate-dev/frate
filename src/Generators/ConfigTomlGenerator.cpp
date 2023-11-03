#include "./Generators.hpp"
#include "../Command/Command.hpp"
#include <string>
#include <iostream>
#include <regex>
#include <termcolor/termcolor.hpp>

namespace Generators::ConfigToml {






  bool readUserInput(std::shared_ptr<Command::Context> ctx, std::shared_ptr<Config> config_toml) {
    validateLang:
    if(!validateLang("📚Language->" + ctx->lang + " : ", ctx, config_toml)) {
      std::cout << termcolor::red << "Invalid language - retry" << termcolor::reset << std::endl;
      goto validateLang;
    }
    validateProjectName:
    if (!validateProjectName("📖Project name->" + ctx->project_name + " : ", ctx, config_toml)) {
      std::cout << termcolor::red << "Invalid project name - retry" << termcolor::reset << std::endl;
      goto validateProjectName;
    }
    validateCmakeVersion:
    if (!validateCmakeVersion("🔨Cmake version->" + ctx->cmake_version + " : ", ctx, config_toml)) {
      std::cout << termcolor::red << "Invalid cmake version - retry" << termcolor::reset << std::endl;
      goto validateCmakeVersion;
    }
    validateProjectVersion: 
    if (!validateProjectVersion("🗃️Version->" + ctx->project_version + " : ", ctx, config_toml)) {
      std::cout << termcolor::red << "Invalid project version - retry" << termcolor::reset << std::endl;
      goto validateProjectVersion;
    }
    validateLanguageVersion:
    if (!validateLanguageVersion("📰Language Standard->" + ctx->lang_version + " : ", ctx, config_toml)) {
      std::cout << termcolor::red << "Invalid language version - retry" << termcolor::reset << std::endl;
      goto validateLanguageVersion;
    }
    validateCompiler:
    if (!validateCompiler("💽Compiler->", ctx, config_toml)) {
      std::cout << termcolor::red << "Invalid compiler - retry" << termcolor::reset << std::endl;
      goto validateCompiler;
    }
    validateSourceDir:
    if (!validateSourceDir("⛲Source Dir->" + ctx->src_dir + " : ", ctx, config_toml)) {
      std::cout << termcolor::red << "Invalid source directory - retry" << termcolor::reset << std::endl;
      goto validateSourceDir;
    }
    validateBuildDir:
    if (!validateBuildDir("🛠️Build Dir->" + ctx->build_dir + " : ", ctx, config_toml)) {
      std::cout << termcolor::red << "Invalid build directory - retry" << termcolor::reset << std::endl;
      goto validateBuildDir;
    }
    validateIncludeDir:
    if (!validateIncludeDir("🫃Include Dir->" + ctx->include_dir + " : ", ctx, config_toml)) {
      std::cout << termcolor::red << "Invalid include directory - retry" << termcolor::reset << std::endl;
      goto validateIncludeDir;
    }
    return true;
  }


  bool writeConfig(std::shared_ptr<Command::Context> &ctx) {
    toml::array authors = toml::array{};
    toml::array flags = toml::array{};
    for (auto &flag : ctx->flags) {
      flags.push_back(flag);
    }
    for (const auto &author : ctx->authors) {
      authors.push_back(author);
    }
    toml::table table = toml::table{
      {"project",
        toml::table{
          {"cmake_version", ctx->cmake_version},
          {"include_dir", ctx->include_dir},
          {"project_version", ctx->project_version},
          {"compiler", ctx->compiler},
          {"project_name", ctx->project_name},
          {"authors", authors},
          {"src_dir", ctx->src_dir},
          {"build_dir", ctx->build_dir},
          {"lang", ctx->lang},
          {"lang_version", ctx->lang_version},
          {"cflags", flags},
        }},
    };

    toml::table deps_table = toml::table{{"dependencies", toml::table{}}};

    for (Command::dependency dep : ctx->dependencies) {
      
      toml::array deps_values = toml::array{};
      deps_values.push_back(dep.url);
      deps_values.push_back(dep.version);
      deps_values.push_back(dep.target_link);
      std::cout << deps_values << std::endl;
      deps_table["dependencies"].as_table()->insert(dep.name, deps_values);
    }



    std::ofstream file;
    std::string file_name = "config.toml";
    file.open(ctx->project_path / file_name);
    file << table;
    file << '\n';
    file << deps_table;
    file << '\n';
    file.close();
    return true;
  }

  } // namespace Generators::ConfigToml
