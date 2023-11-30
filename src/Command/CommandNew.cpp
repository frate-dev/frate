#include <Frate/Command.hpp>
#include <cxxopts.hpp>
#include <fstream>
#include <iostream> 
#include <string>
#include <inja.hpp>
#include <Frate/Generators.hpp>
#include <Frate/Utils/General.hpp>
#include <Frate/Wizards.hpp>
#include <git2.h>

namespace Command {
using Utils::CLI::Prompt;
using Utils::CLI::Ansi::RED;
 bool downloadCMakeListsTemplate(std::shared_ptr<Command::Project> _){

    std::cout << "Downloading CMakeLists.txt" << std::endl;
    std::ofstream file;
    json CMakeListsTemplateIndex = Utils::fetchJson("https://github.com/frate-dev/templates/releases/latest/download/index.json");
    try{
      file.open(static_cast<std::string>(std::getenv("HOME")) + "/.config/frate/" + "templates.json");
      file << CMakeListsTemplateIndex.dump(2);
    }catch(...){
      Utils::debug("Error while opening file: templates.json");
      return false;
    }
      
    return true;
  }
bool downloadCMakeListsTemplate(Interface* inter){
  std::shared_ptr<Project> pro = inter->pro;
  std::string template_index = static_cast<std::string>(std::getenv("HOME"))+ "/.config/frate/templates.json";

  std::filesystem::create_directories(static_cast<std::string>(std::getenv("HOME")) + "/.config/frate");
  std::ifstream  template_index_file;

  if (!std::filesystem::exists(template_index)){
    downloadCMakeListsTemplate(pro); 
  }
  std::string repo_url;
  std::cout << "file: " << template_index << std::endl;
  json templateIndex = json::parse(std::ifstream(template_index));
  std::cout << templateIndex.dump(2) << std::endl;
  for(json temp: templateIndex){
    if (temp["name"] == pro->template_name){
      repo_url = temp["git"];
    }
  }


 git_repository *repo = NULL; 
  std::cout << "Cloning " << repo_url << " into " << (pro->project_path / "templates").c_str() << std::endl;
  git_clone(&repo, repo_url.c_str(), (pro->project_path / "templates").c_str(), NULL);
  std::filesystem::remove_all(pro->project_path / "templates" / ".git");
  return true;
}

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

    if(std::filesystem::exists(pro->project_path / pro->src_dir)){
      Prompt *overwrite_prompt = new Prompt("src directory already exists, overwrite?");
      overwrite_prompt->Color(RED);
      overwrite_prompt->IsBool();
      overwrite_prompt->Run();
      if(!overwrite_prompt->Get<bool>().second){
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
    std::string file_path = pro->project_path / pro->src_dir;
    try{

      std::filesystem::rename(pro->project_path / "templates" / "src", file_path);
      inja::Environment env;
      std::string file_content = env.render_file(pro->project_path / pro->src_dir / "main.tmpl", pro->toJson());
      std::ofstream file_stream;
      file_stream.open(pro->project_path / pro->src_dir / "main.cpp");
      file_stream << file_content;
      std::filesystem::remove(pro->project_path / pro->src_dir / "main.tmpl");


    }catch(std::exception &e){
      Utils::debug(e.what());
      return false;
    }
    file_stream.close();
    return true;
  }
  bool createHelloWorldC(std::shared_ptr<Project> pro) {
    if(std::filesystem::exists(pro->project_path / pro->src_dir)){
      Prompt *overwrite_prompt = new Prompt("src directory already exists, overwrite?");
      overwrite_prompt->IsBool();
      overwrite_prompt->Run();
      overwrite_prompt->Color(RED);
      if(!overwrite_prompt->Get<bool>().second){
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
    std::string file_path = pro->project_path / pro->src_dir ;
    try{

      std::filesystem::rename(pro->project_path / "templates" / "src", file_path);
      inja::Environment env;
      std::string file_content = env.render_file(pro->project_path / pro->src_dir / "main.tmpl", pro->toJson());
      std::ofstream file_stream;
      file_stream.open(pro->project_path / pro->src_dir / "main.c");
      std::filesystem::remove(pro->project_path / pro->src_dir / "main.tmpl");
      file_stream << file_content;
    }catch(std::exception &e){
      Utils::debug(e.what());
      return false;
    }
    file_stream.close();
    return true;
  }
  void gitInit(Interface *inter){
    #ifdef RELEASE 
      Generators::GitIgnore::create(inter->pro);
      //TODO: make this work on windows
      git_repository *repo = nullptr;
      git_repository_init(&repo, inter->pro->project_path.string().c_str(),0);
      //int gitinit = Utils::hSystem("cd "+inter->pro->project_path.string()+";git init");
      
      
      //if(gitinit != 0){
      //  std::cout << "git init failed" << std::endl;
      //}
    #endif
    (void)inter;
  }

  bool createProjectWizard(Interface *inter){
    createJson(inter->pro);
    inter->LoadProjectJson();
    downloadCMakeListsTemplate(inter);
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
    std::string file_name = "frate-project.json";

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

  bool initTemplate(){
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
    std::string file_name = "frate-project.json";
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


    downloadCMakeListsTemplate(this);
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
      std::cout << "Creating frate-project.json" << ENDL;
      if(!createProjectWizard(this)){
        return false;
      }
    }
    Generators::Readme::create(pro);
    if(project_type == ProjectType::EXECUTABLE){
      if(!Generators::CMakeList::createCMakeLists(pro)){
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
    std::cout << "CMakeLists.txt created" << ENDL;
    std::cout << "Initializing git" << ENDL;
    gitInit(this);
    std::cout << "Done" << ENDL;

    return true;
  }
  } // namespace Command
