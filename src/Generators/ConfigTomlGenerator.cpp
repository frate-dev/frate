#include "./Generators.hpp"
#include "../Command/Command.hpp"
#include <string>
#include <iostream>
#include <regex>

namespace Generators::ConfigToml {
  /*
   * Validates the cmake version
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the version is valid
   */
  bool cmakeVersion(std::string prefix, std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml) {
    std::cout << prefix;
#ifndef TEST
    std::getline(std::cin, config_toml->cmake_version);
#endif
    //If the version is empty we're gonna set it
    if(config_toml->cmake_version == "") {
      goto end;
    }
    //Checking if the version is x.x.x 
    if(std::regex_match(config_toml->cmake_version, std::regex("^[0-4]+\\.[0-9]+\\.[0-9]+$"))) {
      goto end;
    }
    //Checking if the version is x.x
    if(std::regex_match(config_toml->cmake_version, std::regex("^[0-4]+\\.[0-9]+$"))) {
      goto end;
    }
    return false;
    end:
      ctx->cmake_version = config_toml->cmake_version == "" ? ctx->cmake_version : config_toml->cmake_version;
    
    return true;
  }

  /*
   * Validates the project name
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the project name is valid
   */
  bool projectName(std::string prefix, std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml) {
    std::cout << prefix;
#ifndef TEST
    std::getline(std::cin, config_toml->project_name);
#endif
    //If the name is empty we're gonna set it
    if(config_toml->project_name == "") {
      goto end;

    }
    //Checking if the name is valid
    if(std::regex_match(config_toml->project_name, std::regex("^[a-zA-Z0-9_-]+$"))) {
      goto end;
    }
    return false;
    end:
      ctx->project_name = config_toml->project_name == "" ? ctx->project_name : config_toml->project_name;

    return true;
  }

  /*
   * Validates the project version
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the project version is valid
   */
  bool projectVersion(std::string prefix,  std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml) {
    std::cout << prefix;
#ifndef TEST
      std::getline(std::cin, config_toml->project_version);
#endif
    if(config_toml->project_version == "") {
      goto end;
    }
    //Checking if the version is x.x.x 
    if(std::regex_match(config_toml->project_version, std::regex("^[0-9]+\\.[0-9]+\\.[0-9]+$"))) {
      goto end;
    }
    //Checking if the version is x.x
    if(std::regex_match(config_toml->project_version, std::regex("^[0-9]+\\.[0-9]+$"))) {
      goto end;
    }
    return false;
    //If the version is empty we're gonna set it
    end:
      ctx->project_version = config_toml->project_version == "" ? ctx->project_version : config_toml->project_version;
    
    return true;

  }
  
  /*
   * Validates the language version
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the language version is valid
   */
  bool languageVersion(std::string prefix, std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml) {
    std::cout << prefix;
#ifndef TEST
      std::getline(std::cin, config_toml->lang_version);
#endif
    std::vector<std::string> valid_c_versions = {"89", "90", "94", "99", "11", "18"};
    std::vector<std::string> valid_cpp_versions = {"98", "03", "11", "14", "17", "20","23"};

    //If the version is empty we're gonna set it
    if(config_toml->lang_version == "") {
      goto end;
    }
    for(std::string version : valid_c_versions) {
      if(config_toml->lang_version == version) {
        goto end;
        break;
      }
    }
    for(std::string version : valid_cpp_versions) {
      if(config_toml->lang_version == version) {
        goto end;
        break;
      }
    }
    return false;
    end:
      ctx->lang_version = config_toml->lang_version == "" ? ctx->lang_version : config_toml->lang_version;
    
    return true;
  }

  /*
   * Validates the compiler 
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the language is valid
   */
  bool compiler(std::string prefix, std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml) {
    std::cout << prefix;
#ifndef TEST
      std::getline(std::cin, config_toml->compiler);
#endif
    //If the compiler is empty we're gonna set it
    if(config_toml->compiler == "") {
      goto end;
    }
    //check if the compiler is valid for c++
    if (config_toml->compiler == "g++" || config_toml->compiler == "clang++") {
      if(config_toml->lang == "cpp" || config_toml->lang == "c++"){
        goto end;
      }
    }
    //check if the compiler is valid for c
    if (config_toml->compiler == "gcc" || config_toml->compiler == "clang" ||   config_toml->compiler == "msvc" || config_toml->compiler == "icc" || config_toml->compiler == "tcc" || config_toml->compiler == "emcc" || config_toml->compiler == "clang++" || config_toml->compiler == "g++") {
      if(config_toml->lang == "c"){
        goto end;
      }
    }
    return false;
    end:
      ctx->compiler = config_toml->compiler == "" ? ctx->compiler : config_toml->compiler;
    
    return true;
  }

  /*
   * Validates the source directory
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the source directory is valid
   */
  bool sourceDir(std::string prefix, std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml) {
    std::cout << prefix;
#ifndef TEST
      std::getline(std::cin, config_toml->src_dir);
#endif
    //If the source directory is empty we're gonna set it
    if(config_toml->src_dir == "") {
      goto end;
    }
    //check if the source directory is valid
    if (std::regex_match(config_toml->src_dir, std::regex("^[a-zA-Z0-9_-]+$"))) {
      goto end;
    }
    if (config_toml->src_dir == "src") {
      goto end;
    }
    return false;
    end:
      ctx->src_dir = config_toml->src_dir == "" ? ctx->src_dir : config_toml->src_dir;
    
    return true;
  }

  /*
   * Validates the build directory
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the build directory is valid
   */
  bool buildDir(std::string prefix, std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml) { 
    std::cout << prefix;
#ifndef TEST
      std::getline(std::cin, config_toml->build_dir);
#endif
    //If the build directory is empty we're gonna set it
    if(config_toml->build_dir == "") {
      goto end;
    }
    if (config_toml->build_dir == "build") {
      goto end;
    }
    //check if the build directory is valid
    if (std::regex_match(config_toml->build_dir, std::regex("^[a-zA-Z0-9_-]+$"))) {
      goto end;
    }
    return false;
    end:
      ctx->build_dir = config_toml->build_dir == "" ? ctx->build_dir : config_toml->build_dir;
    
    return true;
  }

  /*
   * Reads the data from the user
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the data is valid
   */
  bool includeDir(std::string prefix, std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml) {
    std::cout << prefix;
#ifndef TEST
      std::getline(std::cin, config_toml->include_dir);
#endif
    //If the include directory is empty we're gonna set it
    if(config_toml->include_dir == "") {
      goto end;
    }
    //check if the include directory is valid
    if (std::regex_match(config_toml->include_dir, std::regex("^[a-zA-Z0-9_-]+$"))) {
      goto end;
    }

    return false;
    end:
      ctx->include_dir = config_toml->include_dir == "" ? ctx->include_dir : config_toml->include_dir;
    
    return true;
  }


  bool readData(std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml) {
    if (!projectName("📖Project name: (" + ctx->project_name + "): ", ctx, config_toml)) {
      std::cout << "Invalid project name - retry" << std::endl;
      readData(ctx, config_toml);
    }
    if (!cmakeVersion("🔨Cmake version: (" + ctx->cmake_version + "): ", ctx, config_toml)) {
      std::cout << "Invalid cmake version - retry" << std::endl;
      readData(ctx, config_toml);
    }
  
    if (!projectVersion("🗃️Version: (" + ctx->project_version + "): ", ctx, config_toml)) {
      std::cout << "Invalid project version - retry" << std::endl;
      readData(ctx, config_toml);
    }
    if (!languageVersion("📰Language Standard: (" + ctx->lang_version + "): ", ctx, config_toml)) {
      std::cout << "Invalid language version - retry" << std::endl;
      readData(ctx, config_toml);
    }
    if (!compiler("💽Compiler: (" + ctx->compiler + "): ", ctx, config_toml)) {
      std::cout << "Invalid compiler - retry" << std::endl;
      readData(ctx, config_toml);
    }
    if (!sourceDir("⛲Source Dir: (" + ctx->src_dir + "): ", ctx, config_toml)) {
      std::cout << "Invalid source directory - retry" << std::endl;
      readData(ctx, config_toml);
    }
    if (!buildDir("🛠️Build Dir: (" + ctx->build_dir + "): ", ctx, config_toml)) {
      std::cout << "Invalid build directory - retry" << std::endl;
      readData(ctx, config_toml);
    }
    if (!includeDir("🫃Include Dir: (" + ctx->include_dir + "): ", ctx, config_toml)) {
      std::cout << "Invalid include directory - retry" << std::endl;
      readData(ctx, config_toml);
    }
    return true;
  }


  bool writeConfig(std::shared_ptr<Command::Context> &ctx) {
    toml::array authors = toml::array{};
    toml::array flags = toml::array{};
    for (auto &flag : ctx->flags) {
      flags.push_back(flag);
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

    for (Command::dependency &dep : ctx->dependencies) {

      toml::array deps_values = toml::array{};
      deps_values.push_back(dep.url);
      deps_values.push_back(dep.version);
      deps_table["dependencies"].as_table()->insert(dep.name, deps_values);
    }



    std::ofstream file;
    std::string file_name = "config.toml";
#ifdef DEBUG
    file_name = "build/config.toml";
    std::cout << "Writing config.toml to " << ctx->project_path / file_name
      << std::endl;
#endif
    file.open(ctx->project_path / file_name);
    file << table;
    file << '\n';
    file << deps_table;
    file << '\n';
    file.close();
    return true;
  }



}
