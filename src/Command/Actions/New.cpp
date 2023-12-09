#include <cxxopts.hpp>
#include <filesystem>
#include <fstream>
#include <iostream> 
#include <string>
#include <inja.hpp>
#include <Frate/Generators.hpp>
#include <Frate/Utils/General.hpp>
#include <Frate/Wizards.hpp>
#include "Frate/Command/Actions/Help.hpp"
#include <git2.h>

namespace Frate::Command::New {
using Utils::CLI::Prompt;
using Utils::CLI::Ansi::RED;

bool options(Interface* inter) {
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




  void gitInit(Interface *inter){
    #ifdef RELEASE 
      Generators::GitIgnore::create(inter);
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
    if(!Generators::Project::create(inter->pro)){
      return false;
    } 
    gitInit(inter);
    return true;
  }




  bool initTemplate(){
    return true;
  }

  bool run(Interface* inter) {
    options(inter);
#ifdef RELEASE
    if(!inter->args->count("subcommand")){
      Help::run(inter);
      return false;
    }
    std::string directory = inter->args->operator[]("subcommand").as<std::string>();
    if (directory != "."){
      std::filesystem::create_directories(directory);
      std::filesystem::current_path(directory);
      inter->pro->project_path = std::filesystem::current_path();
    }
#endif
    std::string project_config_file = "frate-project.json";
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
    if(inter->args->operator[]("name").count() > 0){
      project_name = inter->args->operator[]("name").as<std::string>();
    }
    if(inter->args->operator[]("type").count() > 0){
      inter->pro->project_type = inter->args->operator[]("type").as<std::string>();
      // if(!ProjectType::validate(inter->pro->project_type)){
      //   Frate::error << "Invalid project type" << ENDL;
      //   return false;
      // }
    }
    if(inter->args->operator[]("language").count() > 0){
      language = inter->args->operator[]("language").as<std::string>();
    }
    if(inter->args->operator[]("defaults").count() > 0){
      defaults = inter->args->operator[]("defaults").as<bool>();
    }


    /*
     * TODO: Do some validation on user inputs
     */



    inter->pro->project_name = project_name;
    inter->pro->lang = language;


    //downloadCMakeListsTemplate(inter);
    if(defaults){
      Frate::info << "Using defaults" << ENDL;
      Frate::info << "Creating project" << ENDL;
      inter->pro->project_type = "default-executable";
      if(!createProjectWizard(inter)){
        return false;
      }
    }else{
      Frate::info << "Creating project" << ENDL;
      Frate::info << "Creating frate-project.json" << ENDL;
      if(!createProjectWizard(inter)){
        return false;
      }
    }
    return true;
  }
  } // namespace Command
