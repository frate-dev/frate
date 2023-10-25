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
    std::cout << prefix << ENDL;
    std::getline(std::cin, config_toml->cmake_version);
    bool valid_version = false;
    //Checking if the version is x.x.x 
    if(std::regex_match(config_toml->cmake_version, std::regex("^[0-9]+\\.[0-9]+\\.[0-9]+$"))) {
      valid_version = true;
    }
    //Checking if the version is x.x
    if(std::regex_match(config_toml->cmake_version, std::regex("^[0-9]+\\.[0-9]+$"))) {
      valid_version = true;
    }
    //If the version is empty we're gonna set it
    if(config_toml->cmake_version == "") {
      valid_version = true;
    }

    ctx->cmake_version = config_toml->cmake_version == "" ? ctx->cmake_version : config_toml->cmake_version;
    
    return valid_version;
  }

  /*
   * Validates the project name
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the project name is valid
   */
  bool projectName(std::string prefix, std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml) {
    std::cout << prefix << ENDL;
    std::getline(std::cin, config_toml->project_name);
    bool valid_name = false;
    //Checking if the name is valid
    if(std::regex_match(config_toml->project_name, std::regex("^[a-zA-Z0-9_-]+$"))) {
      valid_name = true;
    }
    //If the name is empty we're gonna set it
    if(config_toml->project_name == "") {
      valid_name = true;
      return valid_name;
    }

    ctx->project_name = config_toml->project_name == "" ? ctx->project_name : config_toml->project_name;
    return valid_name;
  }

  /*
   * Validates the project version
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the project version is valid
   */
  bool projectVersion(std::string,  std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml) {
    std::cout << "🗃️Version: (" << ctx->project_version << "): ";
    std::getline(std::cin, config_toml->project_version);
    bool valid_version = false;
    //Checking if the version is x.x.x 
    if(std::regex_match(config_toml->project_version, std::regex("^[0-9]+\\.[0-9]+\\.[0-9]+$"))) {
      valid_version = true;
    }
    //Checking if the version is x.x
    if(std::regex_match(config_toml->project_version, std::regex("^[0-9]+\\.[0-9]+$"))) {
      valid_version = true;
    }
    //If the version is empty we're gonna set it
    if(config_toml->project_version == "") {
      valid_version = true;
    }

    ctx->project_version = config_toml->project_version == "" ? ctx->project_version : config_toml->project_version;
    
    return valid_version;

  }
  
  /*
   * Validates the language version
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the language version is valid
   */
  bool languageVersion(std::string, std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml) {
    std::cout << "📰Language Standard: (" << ctx->lang_version << "): ";
    std::getline(std::cin, config_toml->lang_version);
    bool valid_version = false;

    //If the version is empty we're gonna set it
    if(config_toml->lang_version == "") {
      valid_version = true;
    }

    //check if the version is valid for c++
    if (config_toml->lang_version == "98" || config_toml->lang_version == "03" ||   config_toml->lang_version == "11" || config_toml->lang_version == "14" || config_toml->lang_version == "17" || config_toml->lang_version == "20" ) {
      if(config_toml->lang == "c"){
        valid_version = false;
      }
      valid_version = true;
    }
    //check if the version is valid for c
    if (config_toml->lang_version == "89" || config_toml->lang_version == "90" ||   config_toml->lang_version == "94" || config_toml->lang_version == "99" || config_toml->lang_version == "11" || config_toml->lang_version == "18" ) {
      if(config_toml->lang == "cpp"){
        valid_version = false;
      }
      valid_version = true;
    }

    ctx->lang_version = config_toml->lang_version == "" ? ctx->lang_version : config_toml->lang_version;
    
    return valid_version;
  }

  /*
   * Validates the compiler 
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the language is valid
   */
  bool compiler(std::string, std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml) {
    std::cout << "💽Compiler: (" << ctx->compiler << "): ";
    std::getline(std::cin, config_toml->compiler);
    bool valid_compiler = false;
    //If the compiler is empty we're gonna set it
    if(config_toml->compiler == "") {
      valid_compiler = true;
    }
    //check if the compiler is valid for c++
    if (config_toml->compiler == "g++" || config_toml->compiler == "clang" ||   config_toml->compiler == "msvc" || config_toml->compiler == "icc" || config_toml->compiler == "tcc" || config_toml->compiler == "emcc" ) {
      if(config_toml->lang == "c"){
        valid_compiler = false;
      }
      valid_compiler = true;
    }
    //check if the compiler is valid for c
    if (config_toml->compiler == "gcc" || config_toml->compiler == "clang" ||   config_toml->compiler == "msvc" || config_toml->compiler == "icc" || config_toml->compiler == "tcc" || config_toml->compiler == "emcc" ) {
      if(config_toml->lang == "cpp"){
        valid_compiler = false;
      }
      valid_compiler = true;
    }

    ctx->compiler = config_toml->compiler == "" ? ctx->compiler : config_toml->compiler;
    
    return valid_compiler;
  }

  /*
   * Validates the source directory
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the source directory is valid
   */
  bool sourceDir(std::string, std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml) {
    std::cout << "⛲Source Dir: (" << ctx->src_dir << "): ";
    std::getline(std::cin, config_toml->src_dir);
    bool valid_src_dir = false;
    //If the source directory is empty we're gonna set it
    if(config_toml->src_dir == "") {
      valid_src_dir = true;
    }
    //check if the source directory is valid
    if (std::regex_match(config_toml->src_dir, std::regex("^[a-zA-Z0-9_-]+$"))) {
      valid_src_dir = true;
    }
    if (config_toml->src_dir == "src") {
      valid_src_dir = false;
    }

    ctx->src_dir = config_toml->src_dir == "" ? ctx->src_dir : config_toml->src_dir;
    
    return valid_src_dir;
  }

  /*
   * Validates the build directory
   * @param prefix: the prefix of the message
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the build directory is valid
   */
  bool buildDir(std::string, std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml) { std::cout << "🛠️Build Dir: (" << ctx->build_dir << "): ";
    std::getline(std::cin, config_toml->build_dir);
    bool valid_build_dir = false;
    //If the build directory is empty we're gonna set it
    if(config_toml->build_dir == "") {
      valid_build_dir = true;
    }
    //check if the build directory is valid
    if (std::regex_match(config_toml->build_dir, std::regex("^[a-zA-Z0-9_-]+$"))) {
      valid_build_dir = true;
    }

    ctx->build_dir = config_toml->build_dir == "" ? ctx->build_dir : config_toml->build_dir;
    
    return valid_build_dir;
  }

  /*
   * Reads the data from the user
   * @param ctx: the context of the command
   * @param config_toml: the config toml context
   * @return: true if the data is valid
   */
  bool includeDir(std::string, std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml) {
    std::cout << "🫃Include Dir: (" << ctx->include_dir << "): ";
    std::getline(std::cin, config_toml->include_dir);
    bool valid_include_dir = false;
    //If the include directory is empty we're gonna set it
    if(config_toml->include_dir == "") {
      valid_include_dir = true;
    }
    //check if the include directory is valid
    if (std::regex_match(config_toml->include_dir, std::regex("^[a-zA-Z0-9_-]+$"))) {
      valid_include_dir = true;
    }

    ctx->include_dir = config_toml->include_dir == "" ? ctx->include_dir : config_toml->include_dir;
    
    return valid_include_dir;
  }


  bool readData(std::shared_ptr<Command::Context> &ctx, std::shared_ptr<ConfigToml> &config_toml) {
    if (!cmakeVersion("🔨Cmake version: (" + ctx->cmake_version + "): ", ctx, config_toml)) {
      readData(ctx, config_toml);
    }
    if (!projectName("📖Project name: (" + ctx->project_name + "): ", ctx, config_toml)) {
      readData(ctx, config_toml);
    }  
    if (!projectVersion("🗃️Version: (" + ctx->project_version + "): ", ctx, config_toml)) {
      readData(ctx, config_toml);
    }
    if (!languageVersion("📰Language Standard: (" + ctx->lang_version + "): ", ctx, config_toml)) {
      readData(ctx, config_toml);
    }
    if (!compiler("💽Compiler: (" + ctx->compiler + "): ", ctx, config_toml)) {
      readData(ctx, config_toml);
    }
    if (!sourceDir("⛲Source Dir: (" + ctx->src_dir + "): ", ctx, config_toml)) {
      readData(ctx, config_toml);
    }
    if (!buildDir("🛠️Build Dir: (" + ctx->build_dir + "): ", ctx, config_toml)) {
      readData(ctx, config_toml);
    }
    if (!includeDir("🫃Include Dir: (" + ctx->include_dir + "): ", ctx, config_toml)) {
      readData(ctx, config_toml);
    }
    return true;
  }



  bool writeConfig(std::shared_ptr<Command::Context> &ctx){
    toml::array authors = toml::array{};
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
        }},
    };
    std::cout << "📄New Toml File: \n";
    std::cout << table << '\n';
    std::ofstream file;
    std::string file_name = "config.toml";
    #ifdef DEBUG
      file_name  ="build/config.toml";
    #endif

    file.open(ctx->project_path / file_name);
    file << table;
    file << '\n';
    file.close();
    return true;
  }

}
