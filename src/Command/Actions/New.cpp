#include <cxxopts.hpp>
#include <filesystem>
#include <string>
#include <inja.hpp>
#include <Frate/Generators.hpp>
#include <Frate/Utils/General.hpp>
#include <Frate/Wizards.hpp>
#include <Frate/Project.hpp>
#include <Frate/Command/Actions/Help.hpp>
#include "Frate/Command.hpp"

namespace Frate::Command::New {
using std::filesystem::path;
using Utils::CLI::Prompt;
using Utils::CLI::Ansi::RED;

bool options(std::shared_ptr<Interface> inter) {
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




  void gitInit(std::shared_ptr<Interface> inter){
    Generators::GitIgnore::create(inter);
    //TODO: make this work on windows
    Utils::hSystem("git init");
    (void)inter;
  }



  bool createProjectWizard(std::shared_ptr<Interface> inter){
    if(!Generators::Project::create(inter->pro)){
      return false;
    } 
#ifdef RELEASE
    gitInit(inter);
#endif
    return true;
  }




  bool initTemplate(){
    return true;
  }

  bool checkForProject(std::shared_ptr<Interface> inter){
    if(std::filesystem::exists(inter->pro->path / "frate-project.json")){
      Utils::error << "Project already exists" << std::endl;
      return true;
    }
    return false;
  }

  bool promptForOverwrite(std::shared_ptr<Interface> inter){
    (void)inter;
    Prompt prompt("Do you want to overwrite it?");
    prompt.setColor(RED).exitOnFailure();
    prompt.isBool().run();
    auto [valid, value] = prompt.get<bool>();
    if(!valid){
      return false;
    }
    return value;
  }

  bool run(std::shared_ptr<Interface> inter) {
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
      inter->pro->path = std::filesystem::current_path();
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
      inter->pro->type = inter->args->operator[]("type").as<std::string>();
    }
    if(inter->args->operator[]("language").count() > 0){
      language = inter->args->operator[]("language").as<std::string>();
    }
    if(inter->args->operator[]("defaults").count() > 0){
      defaults = inter->args->operator[]("defaults").as<bool>();
    }

    if(checkForProject(inter)){
      if(promptForOverwrite(inter)){

        path tmp_path = Utils::copyToTmpPath(inter->pro->path, "frate-project-");


        for(const auto& entry : std::filesystem::directory_iterator(inter->pro->path)){
          if(entry.path().filename() != "frate"){
            Utils::info << "Removing " << entry.path() << std::endl;
            std::filesystem::remove_all(entry.path());
          }
        }


      }else{


        Utils::error 
          << "Aborting: can't initialize a new project on top of a existing one" << std::endl;
        return false;


      }
    }


    /*
     * TODO: Do some validation on user inputs
     */



    inter->pro->name = project_name;
    inter->pro->lang = language;


    //downloadCMakeListsTemplate(inter);
    if(defaults){
      Utils::info << "Using defaults" << std::endl;
      Utils::info << "Creating project" << std::endl;
      if(inter->pro->type.empty()){
        inter->pro->type = "executable";
      }
      if(!createProjectWizard(inter)){
        return false;
      }
    }else{
      Utils::info << "Creating project" << std::endl;
      Utils::info << "Creating frate-project.json" << std::endl;
      inter->pro->type = "";
      Wizard::Project(inter->pro);
      if(!createProjectWizard(inter)){
        return false;
      }
    }


    return true;
  }
  } // namespace Command
