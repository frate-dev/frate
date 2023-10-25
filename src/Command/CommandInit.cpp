#include "Command.hpp"
#include "toml.hpp"
#include <cxxopts.hpp>
#include <algorithm>
#include <ranges>
#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include "../Generators/Generators.hpp"

namespace Command {

  


bool createToml(std::shared_ptr<Context> ctx) {
  std::shared_ptr<Generators::ConfigToml::ConfigToml>config_toml;
  Generators::ConfigToml::readData(ctx, config_toml);
  return false;
}

bool createHelloWorldCpp(std::shared_ptr<Context> ctx) {
  system("cd build;mkdir src");
  std::ofstream file;
  std::string file_name = ctx->project_path / "src/main.cpp";
  #ifdef DEBUG
      file_name = "build/src/main.cpp";
  #endif
  file.open(ctx->project_path / file_name);
  file << "#include <iostream>\n"
          "int main(){\n"
          " std::cout << \"Hello World\" << std::endl;\n"
          " return 0;\n"
          "}\n";
  return false;
}
bool createHelloWorldC(std::shared_ptr<Context> ctx) {
  system("mkdir src");
  std::ofstream file;
  std::string file_name = "src/main.c";
  #ifdef DEBUG
      file_name = "build/src/main.c";
  #endif
  file.open(ctx->project_path / file_name);
  file << "#include <stdio.h>\n"
          "int main(){\n"
          " printf(\"Hello World\");\n"
          " return 0;\n"
          "}\n";
  return false;
}
bool createCppProject(std::shared_ptr<Context> ctx) {

  createToml(ctx);
  loadPackageToml(ctx);
  Generators::CMakeList::create(ctx);
  createHelloWorldCpp(ctx);
  return true;
}

bool createCProject(std::shared_ptr<Context> ctx) {
  createToml(ctx);
  loadPackageToml(ctx);
  Generators::CMakeList::create(ctx);
  createHelloWorldC(ctx);
  return false;
}


bool defaultTomlCpp(std::shared_ptr<Context> ctx, cxxopts::ParseResult &args) {
  std::string language = args["language"].as<std::string>();
  std::string name = args["name"].as<std::string>();
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


  std::cout << "📄 New Toml File: \n";
  std::cout << table << '\n';

  std::ofstream file;
  std::string file_name = "config.toml";

  #ifdef DEBUG
     file_name = "build/config.toml";
  #endif

  file.open(ctx->project_path / file_name);
  file << table;
  file << '\n';
  file.close();
  return false;
}

bool init(std::shared_ptr<Context> ctx,  cxxopts::ParseResult &args) {
  std::string file_name = "config.toml";
  #ifdef DEBUG
    file_name = "build/config.toml";
  #endif
  file_exists(file_name);
  if(ctx->project_path.empty()){
    ctx->project_path = std::filesystem::current_path();
  }

  std::cout << "project path" << ctx->project_path << ENDL;
  std::cout << "config.toml path" << ctx->project_path / file_name << ENDL;
  std::string current_path =  ctx->project_path.string();


  if (args["skip-init"].as<bool>()) {
    std::string language = args["language"].as<std::string>();
    if (language == "cpp" || language == "c++"){
      defaultTomlCpp(ctx, args);
    }
    else if (language == "c") { 
      //defaultTomlC(ctx, args);
      std::cout << "C is not supported yet" << ENDL;
      exit(-1);
    }

    loadPackageToml(ctx);
    Generators::CMakeList::create(ctx);
    createHelloWorldCpp(ctx);
  } else {
    while (true) {

      std::cout << "Language: ";
      std::getline(std::cin, ctx->lang);
      if (ctx->lang == "cpp" || ctx->lang == "c++") {
        createCppProject(ctx);
        break;
      } else if (ctx->lang == "c") {
        createCProject(ctx);
        break;
      } else if (ctx->lang == "rust") {
        std::cout << "Fuck off" << ENDL;
        break;
      } else if (ctx->lang == "java") {
        std::cout << "'Are you ok? Stop it get some help' - MJ" << ENDL;
        break;
      } else {
        std::cout << "Invalid language" << ENDL;
        return 1;
      }
    }
  }

  return true;
}

} // namespace Command
