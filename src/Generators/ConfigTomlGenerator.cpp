#include "./Generators.hpp"
#include "../Command/Command.hpp"
#include <string>
#include <iostream>
#include <regex>
#include <termcolor/termcolor.hpp>

namespace Generators::ConfigToml {
using std::string;

/*
 * Validates the cmake version
 * @param prefix: the prefix of the message @param ctx: the context of the
 * command
 * @param config_toml: the config toml context
 * @return: true if the version is valid
 */
bool validateCmakeVersion(string prefix, std::shared_ptr<Command::Context> ctx,
                          std::shared_ptr<ConfigToml> config_toml) {
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
  bool validateProjectName(string prefix, std::shared_ptr<Command::Context> ctx, std::shared_ptr<ConfigToml> config_toml) {
    std::cout << prefix;
    #ifndef TEST
      std::getline(std::cin, config_toml->project_name);
    #endif
    //If the name is empty we're gonna set it
    if(config_toml->project_name == "") {
      goto end;
    }
    //Checking if the name is valid
    if ((config_toml->project_name.size() > 255)) {
      return false;
    }
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
  bool validateProjectVersion(string prefix,  std::shared_ptr<Command::Context> ctx, std::shared_ptr<ConfigToml> config_toml) {
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
  bool validateLanguageVersion(string prefix, std::shared_ptr<Command::Context> ctx, std::shared_ptr<ConfigToml> config_toml) {
    std::cout << prefix;
#ifndef TEST
      std::getline(std::cin, config_toml->lang_version);
#endif
    std::vector<string> valid_c_versions = {"89", "90", "94", "99", "11", "18"};
    std::vector<string> valid_cpp_versions = {"98", "03", "11", "14", "17", "20","23"};

    //If the version is empty we're gonna set it
    if(config_toml->lang_version == "") {
      goto end;
    }
    for(string version : valid_c_versions) {
      if(config_toml->lang_version == version) {
        goto end;
        break;
      }
    }
    for(string version : valid_cpp_versions) {
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
  bool validateCompiler(string prefix, std::shared_ptr<Command::Context> ctx, std::shared_ptr<ConfigToml> config_toml) {
    std::vector<string> supportedCppCompilers = {"g++", "clang++"};
    std::vector<string> supportedCCompilers = {"gcc", "clang", "msvc", "icc", "tcc", "emcc"};
    std::cout << prefix << ENDL;
    if(ctx->lang == "cpp" || ctx->lang == "c++"){
      ctx->compiler = supportedCppCompilers[0];
      std::cout << "  Supported Compilers: ( ";
      for(string comp : supportedCppCompilers){
        if(comp == supportedCppCompilers[0]){
          std::cout << " [ " << comp << " ] ";
          continue;
        }else{
          std::cout << comp << " ";
        }
      }
    }
    if(config_toml->lang == "c"){
      ctx->compiler = supportedCCompilers[0];
      std::cout << "  Supported Compilers: ";
      for(string comp : supportedCCompilers){
        if(comp == supportedCCompilers[0]){
          std::cout << " [ " << comp << " ] ";
          continue;
        }else{
          std::cout << comp << " ";
        }
      }
    }
    std::cout << "): ";
#ifndef TEST
      std::getline(std::cin, config_toml->compiler);
#endif
    //If the compiler is empty we're gonna set it
    if(config_toml->compiler == "") {
      goto end;
    }
    if(config_toml->lang == "cpp" || config_toml->lang == "c++"){
      for(string compiler : supportedCppCompilers){
        if(config_toml->compiler == compiler){
          goto end;
        }
      }
    }else if(config_toml->lang == "c"){
      for(string compiler : supportedCCompilers){
        if(config_toml->compiler == compiler){
          goto end;
        }
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
  bool validateSourceDir(string prefix, std::shared_ptr<Command::Context> ctx, std::shared_ptr<ConfigToml> config_toml) {
    std::cout << prefix;
#ifndef TEST
      std::getline(std::cin, config_toml->src_dir);
#endif
    //If the source directory is empty we're gonna set it
    if(config_toml->src_dir == "") {
      goto end;
    }
    if (config_toml->src_dir.size() > 255) {
      return false;
    }
    //check if the source directory is valid
    if (std::regex_match(config_toml->src_dir, std::regex("^[a-zA-Z0-9_-]+$"))) {
      goto end;
    }
    if (config_toml->src_dir == "src") {
      goto end;
    }
    if (sizeof(config_toml->src_dir) > 255) {
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
  bool validateBuildDir(string prefix, std::shared_ptr<Command::Context> ctx, std::shared_ptr<ConfigToml> config_toml) { 
    std::cout << prefix;
#ifndef TEST
      std::getline(std::cin, config_toml->build_dir);
#endif
    //If the build directory is empty we're gonna set it
    if(config_toml->build_dir == "") {
      goto end;
    }
    if (config_toml->build_dir.size() > 255) {
      return false;
    }
    if (config_toml->build_dir == "build") {
      goto end;
    }
    //check if the build directory is valid
    if (std::regex_match(config_toml->build_dir, std::regex("^[a-zA-Z0-9_-]+$"))) {
      goto end;
    }
    if (sizeof(config_toml->build_dir) > 255) {
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
  bool validateIncludeDir(string prefix, std::shared_ptr<Command::Context> ctx, std::shared_ptr<ConfigToml> config_toml) {
    std::cout << prefix;
#ifndef TEST
      std::getline(std::cin, config_toml->include_dir);
#endif
    //If the include directory is empty we're gonna set it
    if(config_toml->include_dir == "") {
      goto end;
    }
    if (config_toml->include_dir.size() > 255) {
      return false;
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

  bool validateLang(string prefix, std::shared_ptr<Command::Context> ctx, std::shared_ptr<ConfigToml> config_toml) {
    std::vector<string> supportedLangs = {"cpp", "c"};
    std::cout << prefix << ENDL;
    std::cout << "  Supported languages: ( ";
    for(string lang : supportedLangs){
      std::cout << lang << " ";
    }
    std::cout << "): ";
#ifndef TEST
      std::getline(std::cin, config_toml->lang);
#endif
    
    if(config_toml->lang == "") {
      goto end;
    }
    for(string lang : supportedLangs){
      if(config_toml->lang == lang){
        goto end;
      }
    }
    return false;
    end:
      ctx->lang = config_toml->lang == "" ? ctx->lang : config_toml->lang;
    return true;
  }


  bool readUserInput(std::shared_ptr<Command::Context> ctx, std::shared_ptr<ConfigToml> config_toml) {
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
    string file_name = "config.toml";
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

  } // namespace Generators::ConfigToml
