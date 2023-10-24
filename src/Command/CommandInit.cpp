#include "Command.hpp"
#include "toml.hpp"
#include <cxxopts.hpp>
#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <string>

namespace Command {

  


bool createToml(std::shared_ptr<Context> ctx) {
  std::ofstream file;
  std::string file_name = "config.toml";
  #ifdef DEBUG
      file_name  ="build/config.toml";
  #endif
    std::string cmake_version;
  std::string project_name;
  std::string project_version;
  std::string include_dir;
  std::string compiler;
  std::string name;
  std::string src_dir;
  std::string build_dir;
  std::string lang;
  std::string lang_version;
  std::string authors_str;

  std::cout << "🔨Cmake version: (" << ctx->cmake_version << "): ";
  std::getline(std::cin, cmake_version);
  std::cout << "📖Project name: (" << ctx->project_name << "): ";
  std::getline(std::cin, project_name);
  std::cout << "🗃️Version: (" << ctx->project_version << "): ";
  std::getline(std::cin, project_version);
  std::cout << "📰Language Standard: (" << ctx->lang_version << "): ";
  std::getline(std::cin, lang_version);
  std::cout << "💽Compiler: (" << ctx->compiler << "): ";
  std::getline(std::cin, compiler);
  std::cout << "⛲Source Dir: (" << ctx->src_dir << "): ";
  std::getline(std::cin, src_dir);
  std::cout << "🛠️Build Dir: (" << ctx->build_dir << "): ";
  std::getline(std::cin, build_dir);
  std::cout << "🫃Include Dir: (" << ctx->include_dir << "): ";
  std::getline(std::cin, include_dir);

  ctx->cmake_version = cmake_version == "" ? ctx->cmake_version : cmake_version;
  ctx->project_name = project_name == "" ? ctx->project_name : project_name;
  ctx->project_version =
      project_version == "" ? ctx->project_version : project_version;
  ctx->lang_version = lang_version == "" ? ctx->lang_version : lang_version;
  ctx->lang = lang == "" ? ctx->lang : lang;
  ctx->compiler = compiler == "" ? ctx->compiler : compiler;
  ctx->src_dir = src_dir == "" ? ctx->src_dir : src_dir;
  ctx->build_dir = build_dir == "" ? ctx->build_dir : build_dir;
  ctx->include_dir = include_dir == "" ? ctx->include_dir : include_dir;

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


  file.open(ctx->project_path / file_name);
  file << table;
  file << '\n';
  file.close();
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
  Command::createCMakelists(ctx);
  createHelloWorldCpp(ctx);
  return true;
}

bool createCProject(std::shared_ptr<Context> ctx) {
  createToml(ctx);
  loadPackageToml(ctx);
  Command::createCMakelists(ctx);
  createHelloWorldC(ctx);
  return false;
}






bool defaultTomlCpp(std::shared_ptr<Context> ctx) {
  ctx->project_name = "TestProject";

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
  if (args["skip-init"].as<bool>()) {
    defaultTomlCpp(ctx);
    loadPackageToml(ctx);
    createCMakelists(ctx);
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
