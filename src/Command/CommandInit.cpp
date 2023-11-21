#include <Frate/Command.hpp>
#include <cstdlib>
#include <cxxopts.hpp>
#include <algorithm>
#include <locale>
#include <ranges>
#include <fstream>
#include <iostream> 
#include <string>
#include <Frate/Generators.hpp>
#include <Frate/Utils/General.hpp>
#include <Frate/Wizards.hpp>

namespace Command {
using Utils::CLI::Prompt;
using Utils::CLI::Ansi::RED;

bool OptionsInit::Init(Interface* inter) {
  inter->InitHeader();
  inter->options->parse_positional({"command", "subcommand"});
  inter->options->add_options()
    ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
    ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage")
    ("d,defaults", "Sets all the defaults", cxxopts::value<bool>()->default_value("false"))
    ("name", "Name of the project", cxxopts::value<std::string>()->default_value("false"))
    ("t,type", "Type of the project", cxxopts::value<std::string>()->default_value(ProjectType::EXECUTABLE))
    ("l,language", "Language of the project", cxxopts::value<std::string>()->default_value("cpp"));
  return inter->parse();
}


bool createJson(std::shared_ptr<Project> pro) {
  // Lucas did it again
  std::shared_ptr<Generators::ConfigJson::Config> config_json =
      std::make_shared<Generators::ConfigJson::Config>();
  // if (!Generators::ConfigJson::readUserInput(pro, config_json)) {
  //   return false;
  // }
  if(!Wizard::Project(pro)){
    return false;
  }
  if (!Generators::ConfigJson::writeConfig(pro)) {
    return false;
  }
  return true;
  }

  bool createHelloWorldCpp(std::shared_ptr<Project> pro) {
    //directory checks
    if(std::filesystem::exists(pro->project_path / pro->src_dir)){
      Prompt<bool> *overwrite_prompt = new Prompt<bool>("src directory already exists, overwrite?");
      overwrite_prompt->Color(RED);
      overwrite_prompt->Run();
      if(!overwrite_prompt->Get()){
        return false;
      }
    }
    //Create src directory
    try{
      std::filesystem::create_directory(pro->project_path / pro->src_dir);
    }catch(std::exception &e){
      Utils::debug(e.what());
      return false;
    }
    std::ofstream file_stream;
    std::filesystem::path file_path = pro->project_path / pro->src_dir / "main.cpp";
    try{
      file_stream.open(file_path);
      file_stream << "#include <iostream>\n"
        "\n"
        "int main(){\n"
        "\tstd::cout << \"Hello World\" << std::endl;\n"
        "\treturn 0;\n"
        "}\n";
    }catch(std::exception &e){
      Utils::debug(e.what());
      return false;
    }
    file_stream.close();
    return true;
  }
  bool createHelloWorldC(std::shared_ptr<Project> pro) {
    if(std::filesystem::exists(pro->project_path / pro->src_dir)){
      Prompt<bool> *overwrite_prompt = new Prompt<bool>("src directory already exists, overwrite?");
      overwrite_prompt->Run();
      overwrite_prompt->Color(RED);
      if(!overwrite_prompt->Get()){
        return false;
      }
    }
    //Create src directory
    try{
      std::filesystem::create_directory(pro->project_path / pro->src_dir);
    }catch(std::exception &e){
      Utils::debug(e.what());
      return false;
    }
    std::ofstream file_stream;
    std::filesystem::path file_path = pro->project_path / pro->src_dir / "main.c";
    try{
      file_stream.open(file_path);
      file_stream << "#include <stdio.h>\n"
        "\n"
        "int main(){\n"
        "\tprintf(\"Hello World\");\n"
        "\treturn 0;\n"
        "}\n";
    }catch(std::exception &e){
      Utils::debug(e.what());
      return false;
    }
    file_stream.close();
    return true;
  }
  void gitInit(Interface *inter){
    #ifndef DEBUG
      Generators::GitIgnore::create(inter->pro);
      //TODO: make this work on windows
      int gitinit = Utils::hSystem("cd "+inter->pro->project_path.string()+";git init");
      if(gitinit != 0){
        std::cout << "git init failed" << std::endl;
      }
    #endif
  }

  bool createProjectWizard(Interface *inter){
    createJson(inter->pro);
    inter->LoadPackageJson();
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
      Utils::debug(e.what());
      return false;
    }
    file << j;
    file << '\n';
    file.close();
    return true;
  }


  bool Interface::init() {
#ifdef RELEASE
    if(!args->count("subcommand")){
      this->help();
      return false;
    }
    std::string directory = args->operator[]("subcommand").as<std::string>();
    if (directory != "."){
      std::filesystem::create_directories(directory);
      std::filesystem::current_path(directory);
      pro->project_path = std::filesystem::current_path();
    }
#endif
    std::string file_name = "config.json";
    std::string project_name = "";
    bool defaults = false;
    std::string language = "cpp";
    std::string project_type = "executable";
#ifdef DEBUG
    project_name = "debug-frate";
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
    if(args->operator[]("defaults").count() > 0){
      defaults = args->operator[]("defaults").as<bool>();
    }


    /*
     * TODO: Do some validation on user inputs
     */



    pro->project_name = project_name;
    pro->lang = language;


    if(defaults){
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
