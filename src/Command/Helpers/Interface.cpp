#include <Frate/Command.hpp>
#include <Frate/Command/Actions/Add.hpp>
#include <Frate/Command/Actions/Set.hpp>
#include <Frate/Command/Actions/Remove.hpp>
#include <Frate/Command/Actions/List.hpp>
#include <Frate/Command/Actions/Search.hpp>
#include "Frate/Command/Actions/Build.hpp"
#include "Frate/Command/Actions/Clean.hpp"
#include "Frate/Command/Actions/FTP.hpp" 
#include "Frate/Command/Actions/Help.hpp"
#include "Frate/Command/Actions/Run.hpp"
#include "Frate/Command/Actions/Update.hpp"
#include "Frate/Command/Actions/Watch.hpp"
#include "Frate/Command/Actions/New.hpp"
#include <Frate/Command/Actions/Completions.hpp>
#include "Frate/Frate.hpp"
#include "Frate/Utils/General.hpp"
#include <Frate/Generators.hpp>
#include "cxxopts.hpp"
#include "termcolor/termcolor.hpp"
#include <Frate/Constants.hpp>  
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <git2.h>

namespace Frate::Command {

  bool OptionsInit::Main(std::shared_ptr<Interface> inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command"});
    inter->options->allow_unrecognised_options().add_options()
      ("command", "Command to run",cxxopts::value<std::string>()->default_value("help"))
      ("verbose", "Verbose output",cxxopts::value<bool>()->default_value("false"))
      ("y,confirm-all", "skip all y/n prompts",cxxopts::value<bool>()->default_value("false"))
      ("version", "Print version");
    return inter->parse();
  }

  bool Interface::parse(){
    try{
      this->args = std::make_shared<cxxopts::ParseResult>(this->options->parse(argc, argv));
      return true;
    } catch (std::exception& e) {
      std::cout << e.what() << std::endl;
      return false;
    }
  }
  bool Interface::InitHeader(){
    try{
      this->options = 
        std::make_shared<cxxopts::Options>("Frate", "A CMake project generator, we suffer so you don't have to!");
      this->options->allow_unrecognised_options();
    } catch (std::exception& e) {
      std::cout << e.what() << std::endl;

      return false;
    }
    return true;
  }

  Interface::Interface(int argc, char** argv){
    this->argc = argc;
    this->argv = argv;
    git_libgit2_init();
    this->pro = std::make_shared<Project>();
#ifdef DEBUG
#ifndef TEST
    verbose_mode = true;
#endif
    std::cout << "DEBUG MODE ENABLED\n";
    pro->path = std::filesystem::current_path() / "build";
#else
    pro->path = std::filesystem::current_path();
#endif

  }
  bool execute(std::shared_ptr<Interface> inter){

    OptionsInit::Main(inter);
    inter->parse();
    if(inter->args->count("version")){
      std::cout << Constants::VERSION << ENDL;
      exit(0);
    }
    if(inter->args->count("verbose")){
      verbose_mode = true;
      std::cout << "Verbose mode enabled" << ENDL;
    }
    if(inter->args->count("confirm-all")){
      inter->confirm_all = true;
      std::cout << "Skipping prompts" << ENDL;
    }

    // if(inter->LoadProjectJson()){
    //   inter->project_present = true;
    // }

    std::string command = inter->args->operator[]("command").as<std::string>();
    std::cout << "Project Path: " << inter->pro->path << ENDL;

    inter->commands.push_back({
      .aliases = {"new", "n"},
      .flags = {"-d,--defaults"}, //TODO: Add flags
      .positional_args = {"project_name/dir"},
      .docs = "Create a new project",
      .callback = &New::run,
      .requires_project = false
    });

    inter->commands.push_back({
      .aliases = {"run"},
      .flags = {"-m,--build-mode","-t,--target"}, //TODO: Add flags
      .docs = "Run the project",
      .callback = &Run::run
    });

    inter->commands.push_back({
      .aliases = {"help", "h"},
      .flags = {}, //TODO: Add flags
      .docs = "Display help",
      .callback = &Help::run,
      .requires_project = false
    });

    inter->commands.push_back({
      .aliases = {"nuke"},
      .flags = {}, //TODO: Add flags
      .docs = "Deletes the entire project",
      .callback = &FTP::run,
      .requires_project = false
    });

    inter->commands.push_back({
      .aliases = {"add"},
      .flags = {}, //TODO: Add flags
      .subcommands = Add::handlers(inter),
      .docs = "add sub command",
      .callback = &Add::run
    });

    inter->commands.push_back({
      .aliases = {"set"},
      .flags = {}, //TODO: Add flags
      .subcommands = Set::handlers(inter),
      .docs = "set sub command",
      .callback = &Set::run
    });

    inter->commands.push_back({
      .aliases = {"search"},
      .flags = {}, //TODO: Add flags
      .subcommands = Search::handlers(inter),
      .docs = "search sub command",
      .callback = &Search::run,
      .requires_project = false
    });
    inter->commands.push_back({
      .aliases = {"list", "ls"},
      .flags = {}, //TODO: Add flags
      .subcommands = List::handlers(inter),
      .docs = "list sub command",
      .callback = &List::run,
      .requires_project = false
    });

    inter->commands.push_back({
      .aliases = {"remove", "rm"},
      .flags = {}, //TODO: Add flags
      .subcommands = Remove::handlers(inter),
      .docs = "remove sub command",
      .callback = &Remove::run
    });

    inter->commands.push_back({
      .aliases = {"update"},
      .flags = {}, //TODO: Add flags
      .subcommands = Update::handlers(inter),
      .docs = "update sub command",
      .callback = &Update::run
    });

    inter->commands.push_back({
      .aliases = {"clean"},
      .flags = {"-c,--cache"}, //TODO: Add flags
      .docs = "clean sub command",
      .callback = &Clean::run
    });

    inter->commands.push_back({
      .aliases = {"build"},
      .flags = {"-m,--mode","-t,--target","-j,--jobs"}, //TODO: Add flags
      .docs = "build sub command",
      .callback = &Build::run
    });
    inter->commands.push_back({
      .aliases = {"completions"},
      .flags = {}, //TODO: Add flags
      .docs = "completions sub command",
      .callback = &Completions::ZshCompletions,
      .implemented = false
    });
    inter->commands.push_back({
      .aliases = {"watch"},
      .flags = {}, //TODO: Add flags
      .docs = "watches the project for changes",
      .callback = &Watch::run
    });


    for(Handler& handler : inter->commands){
      for(std::string& alias : handler.aliases){
        if(alias == command){

          if(handler.requires_project){

            //Check if project loads successfully
            inter->LoadProjectJson();

            //if so refresh the project templates
            Generators::Project::refresh(inter->pro);
          }
          
          //Checks if the callback ran successfully
          if(!handler.callback(inter)){
            return false;
          }

          //Save the project if the command has the possibility of changing it
          if(handler.requires_project){
            inter->pro->save();
          }

          return true;
        }
      }
    }

    //if we get here we know the command was not found
    error << "Error: Command not found: " << command << ENDL;

    return true;
  }
  void renderFlags(std::vector<std::string> flags){
    std::cout << "  [";
    for(std::string flag : flags){
      std::cout << " " << termcolor::bold << termcolor::magenta << flag << termcolor::reset;
    }
    std::cout << " ]";
  }
  void renderPositionals(std::vector<std::string> positionals, bool unlimited_args = false){
    for(std::string positional : positionals){
      if(unlimited_args){
        std::cout << termcolor::bold <<  termcolor::green << " [" << positional << " ...]" << termcolor::reset;
      }else{
        std::cout << termcolor::bold <<  termcolor::green << " [" << positional << "]" << termcolor::reset;
      }
    }
  }
  bool Interface::runCommand(std::string command, std::vector<Handler> &handlers){
    for(Handler handler : handlers){
      for(std::string alias : handler.aliases){
        if(alias == command){
          if(!handler.implemented){
            Frate::error << "Command not implemented: " << command;
            return false;
          }
          if(handler.requires_project && !project_present){
            Frate::error << "Error: Project not found and command: " << command << " requires a project" << ENDL;
            return false;
          }
          if(!handler.callback(shared_from_this())){
            getHelpString(handler);
            return false;
          }
          if(handler.requires_project){
            pro->save();
            Generators::Project::refresh(pro);
          }
          return true;
        }
      }
    }
    std::cout << termcolor::red << "Error: Subcommand not found: " << command << termcolor::reset << ENDL;
    return false;
  }
  void Interface::getHelpString(Handler& handler){
    std::cout << termcolor::bold << termcolor::yellow << handler.aliases[0] << termcolor::reset;
    renderFlags(handler.flags);
    renderPositionals(handler.positional_args);
    std::cout << " - " << handler.docs << ENDL;
  }
  void Interface::getHelpString(std::string name,std::vector<Handler> &handlers, bool is_subcommand){
    size_t index = 0;
    for(Handler handler : handlers){
      index++;
      int alias_str_len = 0;
      std::cout << "  ";
      if(is_subcommand){
        if(index == handlers.size()){
          std::cout << " └── ";
        }
        else{
          std::cout << " ├── ";
        }
        alias_str_len += 4;
      }
      for(std::string alias : handler.aliases){
        alias_str_len += alias.length();
        std::cout << termcolor::bold << termcolor::yellow << alias << termcolor::reset;
        //Render pipes between aliases
        if(alias != handler.aliases.back()){
          alias_str_len += 1;
          std::cout << " | ";
        }
      }
      if(handler.positional_args.size() > 0){
        renderPositionals(handler.positional_args, handler.unlimited_args);
      }
      if(handler.flags.size() > 0){
        renderFlags(handler.flags);
      }
      if(is_subcommand){
        std::cout << " ";
        if(index != handlers.size()){
          std::cout << "\n   │";
        }else{
          std::cout << "\n    ";
        }
        std::cout << "     " << termcolor::blue << handler.docs << termcolor::reset;
      }else{
        if(handler.subcommands.size() > 0){
          std::cout << termcolor::blue << " <subcommand>" << termcolor::reset;

        }else{
          std::cout << " : " << termcolor::blue << handler.docs << termcolor::reset;
        }
      }
      if(!handler.implemented){
        std::cout << termcolor::red << " (Not implemented)" << termcolor::reset << ENDL;
      }else{
        std::cout << ENDL;
      }
      if(handler.subcommands.size() > 0){
        getHelpString(name + " " + handler.aliases[0], handler.subcommands, true);
        std::cout << ENDL;
      }
    }
  }
  Interface::~Interface(){
    git_libgit2_shutdown(); 
  }

} // namespace Command
