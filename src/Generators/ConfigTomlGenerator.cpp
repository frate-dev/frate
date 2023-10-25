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
  bool cmakeVersion(std::string prefix, std::shared_ptr<Command::Context> ctx, std::shared_ptr<ConfigToml> config_toml) {
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

  bool readData(std::shared_ptr<Command::Context> ctx, std::shared_ptr<ConfigToml> config_toml) {
    std::cout << "🔨Cmake version: (" << ctx->cmake_version << "): ";
    std::getline(std::cin, config_toml->cmake_version);
    std::cout << "📖Project name: (" << ctx->project_name << "): ";
    std::getline(std::cin, config_toml->project_name);
    std::cout << "🗃️Version: (" << ctx->project_version << "): ";
    std::getline(std::cin, config_toml->project_version);
    std::cout << "📰Language Standard: (" << ctx->lang_version << "): ";
    std::getline(std::cin, config_toml->lang_version);
    std::cout << "💽Compiler: (" << ctx->compiler << "): ";
    std::getline(std::cin, config_toml->compiler);
    std::cout << "⛲Source Dir: (" << ctx->src_dir << "): ";
    std::getline(std::cin, config_toml->src_dir);
    std::cout << "🛠️Build Dir: (" << ctx->build_dir << "): ";
    std::getline(std::cin, config_toml->build_dir);
    std::cout << "🫃Include Dir: (" << ctx->include_dir << "): ";
    std::getline(std::cin, config_toml->include_dir);
    return true;
  }

  bool setContext(std::shared_ptr<Command::Context> ctx, std::shared_ptr<ConfigToml> config_toml ){
    ctx->cmake_version = config_toml->cmake_version == "" ? ctx->cmake_version :  config_toml->cmake_version;
    ctx->project_name = config_toml->project_name == "" ? ctx->project_name : config_toml->project_name;
    ctx->project_version =
      config_toml->project_version == "" ? ctx->project_version : config_toml->project_version;
    ctx->lang_version = config_toml->lang_version == "" ? ctx->lang_version : config_toml->lang_version;
    ctx->lang = config_toml->lang == "" ? ctx->lang : config_toml->lang;
    ctx->compiler = config_toml->compiler == "" ? ctx->compiler : config_toml->compiler;
    ctx->src_dir = config_toml->src_dir == "" ? ctx->src_dir : config_toml->src_dir;
    ctx->build_dir = config_toml->build_dir == "" ? ctx->build_dir :config_toml->build_dir;
    ctx->include_dir = config_toml->include_dir == "" ? ctx->include_dir : config_toml->include_dir;
    return true;
  }

  bool writeConfig(std::shared_ptr<Command::Context> ctx){
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
