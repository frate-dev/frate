#include "Command.hpp"
#include <cxxopts.hpp>
#include <algorithm>
#include <ranges>
#include <format>
#include <fstream>
#include <iostream> 
#include <string>
#include "../Generators/Generators.hpp"
#include "../Utils/General.hpp"

namespace Command {


bool createJson(std::shared_ptr<Context> ctx) {
  //Lucas did it again
  std::shared_ptr<Generators::ConfigJson::Config>config_json = std::make_shared<Generators::ConfigJson::Config>();
  Generators::ConfigJson::readUserInput(ctx, config_json);
  Generators::ConfigJson::writeConfig(ctx);
  return false;
}

bool createHelloWorldCpp(std::shared_ptr<Context> ctx) {
#ifdef DEBUG
    system("cd build;mkdir src");
#else
    system("mkdir src");
#endif
  std::ofstream file;
  std::string file_name = ctx->project_path / "src/main.cpp";
  std::cout << file_name << std::endl;
  file.open(ctx->project_path / file_name);
  file << "#include <iostream>\n"
          "int main(){\n"
          "\tstd::cout << \"Hello World\" << std::endl;\n"
          "\treturn 0;\n"
          "}\n";
  return false;
}
bool createHelloWorldC(std::shared_ptr<Context> ctx) {
#ifdef DEBUG
    system("cd build;mkdir src");
#else
    system("mkdir src");
#endif
  std::ofstream file;
  std::string file_name = "src/main.c";
  file.open(ctx->project_path / file_name);
  file << "#include <stdio.h>\n"
          "int main(){\n"
          "\tprintf(\"Hello World\");\n"
          "\treturn 0;\n"
          "}\n";
  return false;
}

bool createProject(Interface *inter){
  createJson(inter->ctx);
  inter->LoadPackageJson();
  Generators::CMakeList::create(inter->ctx);
  if(inter->ctx->lang == "cpp"){
    createHelloWorldCpp(inter->ctx);
  }else if(inter->ctx->lang == "c"){
    createHelloWorldC(inter->ctx);
  }
  return false;
}


bool defaultJsonCpp(std::shared_ptr<Context> ctx) {
  using nlohmann::json;
  std::string language = ctx->args->operator[]("language").as<std::string>();
  std::string name = ctx->args->operator[]("name").as<std::string>();
  json j; 
  j["cmake_version"] = ctx->cmake_version;
  j["project_version"] = ctx->project_version;
  j["lang"] = ctx->lang;
  j["lang_version"] = ctx->lang_version;
  j["compiler"] = ctx->compiler;
  j["src_dir"] = ctx->src_dir;
  j["build_dir"] = ctx->build_dir;
  j["include_dir"] = ctx->include_dir;
  j["project_name"] = ctx->project_name;
  j["authors"] = ctx->authors;
  j["project_path"] = ctx->project_path;
  j["project_type"] = ctx->project_type;
  j["project_description"] = ctx->project_description;
  j["flags"] = ctx->flags;
  std::vector<json> deps;
  for (auto &dep : ctx->dependencies) {
    json dep_json;
    dep_json["name"] = dep.name;
    dep_json["url"] = dep.url;
    dep_json["version"] = dep.version;
    dep_json["target_link"] = dep.target_link;
    deps.push_back(dep_json);
  }
  j["dependencies"] = deps;

  std::cout << "📄 New json File: \n";
  std::cout << j << '\n';

  std::ofstream file;
  std::string file_name = "config.json";


  file.open(ctx->project_path / file_name);
  file << j;
  file << '\n';
  file.close();
  return false;
}


bool Interface::init() {
  std::string file_name = "config.json";
  std::string new_project_name = "";
  #ifdef DEBUG
    new_project_name = "DEBUG";
  #else
    new_project_name = Utils::getFolderName();
  #endif

  if(args->operator[]("name").count() > 0){
    new_project_name = args->operator[]("name").as<std::string>();
  }

  ctx->project_name = new_project_name;

  file_exists(file_name);
  if(ctx->project_path.empty()){
    ctx->project_path = std::filesystem::current_path();
  }

  std::cout << "project path" << ctx->project_path << ENDL;
  std::cout << "config.json path" << ctx->project_path / file_name << ENDL;
  std::string current_path =  ctx->project_path.string();


  if (args->operator[]("skip-init").as<bool>()) {
    std::string language = args->operator[]("language").as<std::string>();
    if (language == "cpp" || language == "c++"){
      defaultJsonCpp(ctx);
    }
    else if (language == "c") { 
      //defaultJsonC(ctx, args);
      std::cout << "C is not supported yet" << ENDL;
      exit(-1);
    }

    this->LoadPackageJson();
    Generators::CMakeList::create(ctx);
  } else {
      createProject(this);
  }

  return true;
}

} // namespace Command
