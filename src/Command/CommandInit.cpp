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


bool createJson(std::shared_ptr<Project> pro) {
  //Lucas did it again
  std::shared_ptr<Generators::ConfigJson::Config>config_json = std::make_shared<Generators::ConfigJson::Config>();
  Generators::ConfigJson::readUserInput(pro, config_json);
  Generators::ConfigJson::writeConfig(pro);
  return false;
}

bool createHelloWorldCpp(std::shared_ptr<Project> pro) {
#ifdef DEBUG
    system("cd build;mkdir src");
#else
    system("mkdir src");
#endif
  std::ofstream file;
  std::string file_name = pro->project_path / "src/main.cpp";
  std::cout << file_name << std::endl;
  file.open(pro->project_path / file_name);
  file << "#include <iostream>\n"
          "int main(){\n"
          "\tstd::cout << \"Hello World\" << std::endl;\n"
          "\treturn 0;\n"
          "}\n";
  return false;
}
bool createHelloWorldC(std::shared_ptr<Project> pro) {
#ifdef DEBUG
    system("cd build;mkdir src");
#else
    system("mkdir src");
#endif
  std::ofstream file;
  std::string file_name = "src/main.c";
  file.open(pro->project_path / file_name);
  file << "#include <stdio.h>\n"
          "int main(){\n"
          "\tprintf(\"Hello World\");\n"
          "\treturn 0;\n"
          "}\n";
  return false;
}

bool createProject(Interface *inter){
  createJson(inter->pro);
  inter->LoadPackageJson();
  Generators::CMakeList::create(inter->pro);
  if(inter->pro->lang == "cpp"){
    createHelloWorldCpp(inter->pro);
  }else if(inter->pro->lang == "c"){
    createHelloWorldC(inter->pro);
  }
#ifndef DEBUG
  Generators::GitIgnore::create(inter->pro);
  int gitinit = std::system(("cd "+inter->pro->project_path.string()+";git init").c_str());
  if(gitinit != 0){
    std::cout << termcolor::red << "We had problems initializing your project with git" << termcolor::reset << ENDL;
  }
#endif
  return false;
}


bool defaultJson(std::shared_ptr<Project> pro) {
  using nlohmann::json;
  
  json j = pro->toJson();
  std::cout << "📄 New json File: \n";
  std::cout << j << '\n';

  std::ofstream file;
  std::string file_name = "config.json";


  file.open(pro->project_path / file_name);
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

  pro->project_name = new_project_name;
  //TODO: Stop using this shit
  file_exists(file_name);
  if(pro->project_path.empty()){
    pro->project_path = std::filesystem::current_path();
  }

  std::cout << "project path" << pro->project_path << ENDL;
  std::cout << "config.json path" << pro->project_path / file_name << ENDL;
  std::string current_path =  pro->project_path.string();


  if (args->operator[]("skip-init").as<bool>()) {
    std::string language = args->operator[]("language").as<std::string>();
    if (language == "cpp" || language == "c++"){
      defaultJson(pro);
    }
    else if (language == "c") { 
      defaultJson(pro);
      //TODO: c support is there no need to exitt failing
      std::cout << "C is not supported yet" << ENDL;
      exit(-1);
    }

    this->LoadPackageJson();
    Generators::CMakeList::create(pro);
#ifndef DEBUG
      Generators::GitIgnore::create(pro);
#endif
    
    int gitinit = std::system(("cd "+pro->project_path.string()+";git init").c_str());
    if(gitinit != 0){
      std::cout << termcolor::red << "We had problems initializing your project with git" << termcolor::reset << ENDL;
    }
  } else {
      createProject(this);
  }
  return true;
}

} // namespace Command
