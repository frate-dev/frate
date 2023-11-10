#include "Command.hpp"
#include <cstdlib>
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
    if(!Generators::ConfigJson::readUserInput(pro, config_json)){
      return false;
    }
    if(!Generators::ConfigJson::writeConfig(pro)){
      return false;
    }
    return true;
  }

  bool createHelloWorldCpp(std::shared_ptr<Project> pro) {
    if(Utils::hSystem("cd " + pro->project_path.string() + ";mkdir src") != 0){
      std::cout << "Error creating src directory" << std::endl;
      return false;
    }
    std::ofstream file;
    std::string file_name = pro->project_path / "src/main.cpp";
    std::cout << file_name << std::endl;
    try{
      file.open(pro->project_path / file_name);
      file << "#include <iostream>\n"
        "\n"
        "int main(){\n"
        "\tstd::cout << \"Hello World\" << std::endl;\n"
        "\treturn 0;\n"
        "}\n";
    }catch(std::exception &e){
      std::cout << e.what() << std::endl;
      return false;
    }
    file.close();
    return true;
  }
  bool createHelloWorldC(std::shared_ptr<Project> pro) {
    if(Utils::hSystem("cd " + pro->project_path.string() + ";mkdir src") != 0 ){
      std::cout << "Error creating src directory" << std::endl;
      return false;
    }
    std::ofstream file;
    std::string file_name = "src/main.c";
    try{
      file.open(pro->project_path / file_name);
      file << "#include <stdio.h>\n"
        "\n"
        "int main(){\n"
        "\tprintf(\"Hello World\");\n"
        "\treturn 0;\n"
        "}\n";
    }catch(std::exception &e){
      std::cout << e.what() << std::endl;
      return false;
    }
    file.close();
    return true;
  }

  void gitInit(Interface *inter){
    #ifndef DEBUG
      Generators::GitIgnore::create(inter->pro);
      int gitinit = Utils::hSystem("cd "+inter->pro->project_path.string()+";git init");
      if(gitinit != 0){
        std::cout << "git init failed" << std::endl;
      }
    #endif
  }

  bool createProjectWizard(Interface *inter){
    createJson(inter->pro);
    inter->LoadPackageJson();
    //Generators::CMakeList::create(inter->pro);
    Generators::CMakeList::createCMakeListsExecutable(inter->pro);
    if(inter->pro->lang == "cpp"){
      if(!createHelloWorldCpp(inter->pro)){
        return false;
      }
    }else if(inter->pro->lang == "c"){
      if(!createHelloWorldC(inter->pro)){
        return false;
      }
    }
    gitInit(inter);
    return true;
  }


  bool defaultJson(std::shared_ptr<Project> pro) {
    using nlohmann::json;

    json j = pro->toJson();
    std::cout << "📄 New json File: \n";
    std::cout << j << '\n';

    std::ofstream file;
    std::string file_name = "config.json";

    try{
      file.open(pro->project_path / file_name);
    }catch(std::exception &e){
      std::cout << e.what() << std::endl;
      return false;
    }
    file << j;
    file << '\n';
    file.close();
    return true;
  }


  bool Interface::init() {
    std::string file_name = "config.json";
    std::string project_name = "";
    bool skip_init = false;
    std::string language = "cpp";
    std::string project_type = "executable";
#ifdef DEBUG
    project_name = "debug-cmaker";
#else
    project_name = Utils::getFolderName();
#endif

    /*
     * Parse the arguments
     */
    if(args->operator[]("name").count() > 0){
      project_name = args->operator[]("name").as<std::string>();
    }
    if(args->operator[]("type").count() > 0){
      pro->project_type = args->operator[]("type").as<std::string>();
      if(!ProjectType::validate(pro->project_type)){
        std::cout << termcolor::red << "Invalid project type" << termcolor::reset << ENDL;
        return false;
      }
    }
    if(args->operator[]("language").count() > 0){
      language = args->operator[]("language").as<std::string>();
    }
    if(args->operator[]("skip-init").count() > 0){
      skip_init = args->operator[]("skip-init").as<bool>();
    }


    /*
     * TODO: Do some validation on user inputs
     */



    pro->project_name = project_name;
    pro->lang = language;


    if(skip_init){
      std::cout << "Skipping init" << ENDL;
      std::cout << "Creating project" << ENDL;
      if (language == "cpp" || language == "c++"){
        if(!createHelloWorldCpp(pro)){
          return false;
        }
        if(!defaultJson(pro)){
          return false;
        }
      }
      else if (language == "c") { 
        if(!createHelloWorldC(pro)){
          return false;
        }
        if(!defaultJson(pro)){
          return false;
        }
      }
    }else{
      std::cout << "Creating project" << ENDL;
      std::cout << "Creating config.json" << ENDL;
      if(!createProjectWizard(this)){
        return false;
      }
    }

    if(project_type == ProjectType::EXECUTABLE){
      if(!Generators::CMakeList::createCMakeListsExecutable(pro)){
        return false;
      }
    }else if(project_type == ProjectType::HEADER_ONLY){
      std::cout << "Header only projects are not supported yet" << ENDL;
      return false;
      //TODO: header only
    }else if(project_type == ProjectType::STATIC_LIBRARY){
      std::cout << "Static library projects are not supported yet" << ENDL;
      return false;
      //TODO: static library
    }else if(project_type == ProjectType::SHARED_LIBRARY){
      std::cout << "Shared library projects are not supported yet" << ENDL;
      return false;
      //TODO: shared library
    }

    gitInit(this);


    return true;
  }
} // namespace Command
