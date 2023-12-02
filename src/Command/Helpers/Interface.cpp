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
#include "Frate/Utils/General.hpp"
#include "cxxopts.hpp"
#include "termcolor/termcolor.hpp"
#include <Frate/Constants.hpp>  
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <git2.h>

namespace Frate::Command {

  bool OptionsInit::Main(Interface *inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command"});
    inter->options->allow_unrecognised_options().add_options()(
        "command", "Command to run",
        cxxopts::value<std::string>()->default_value("help"))(
        "v,verbose", "Verbose output",
        cxxopts::value<bool>()->default_value("false"))(
        "y,confim-all", "skip all y/n prompts",
        cxxopts::value<bool>()->default_value("false"))("h,help", "Print usage")
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
      this->options = std::make_shared<cxxopts::Options>("Frate", "A CMake project generator, we suffer so you don't have to!");
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
    OptionsInit::Main(this);
    this->parse();
    if(this->args->count("version")){
      std::cout << Constants::VERSION << ENDL;
      exit(0);
    }
    //After the parse we can set the context args
    this->pro->args = this->args;

#ifdef DEBUG
    std::cout << "DEBUG MODE ENABLED\n";
#endif
#ifdef DEBUG
    pro->project_path = std::filesystem::current_path() / "build";
#else
    pro->project_path = std::filesystem::current_path();
#endif
  }
  bool Interface::execute(){
    if(LoadProjectJson()){
      project_present = true;
    }
    if(this->args->count("yes")){
      this->skip_prompts = true;
      std::cout << "Skipping prompts" << ENDL;
    }
    else{
      this->skip_prompts = false;
    }

    // if(!this->args->count("command")){
    //   this->help();
    std::string command = this->args->operator[]("command").as<std::string>();



    std::cout << "Project Path: " << pro->project_path << ENDL;
    commands = {

      Handler{
        .aliases = {"new", "n"},
        .flags = {"-d,--defaults"}, //TODO: Add flags
        .positional_args = {"project_name/dir"},
        .docs = "Create a new project",
        .callback = [this](){
          return New::run(this);
        },
        .requires_project = false
      },

      Handler{
        .aliases = {"run"},
        .flags = {"-m,--build-mode","-t,--target"}, //TODO: Add flags
        .docs = "Run the project",
        .callback = [this](){
          return Run::run(this);
        }
      },

      Handler{
        .aliases = {"help", "h"},
        .flags = {}, //TODO: Add flags
        .docs = "Display help",
        .callback = [this](){
          return Help::run(this);
        },
        .requires_project = false
      },

      Handler{
        .aliases = {"nuke"},
        .flags = {}, //TODO: Add flags
        .docs = "Deletes the entire project",
        .callback = [this](){
          return FTP::run(this);
        },
        .requires_project = false
      },

      Handler{
        .aliases = {"add"},
        .flags = {}, //TODO: Add flags
        .subcommands = Add::handlers(this),
        .docs = "add sub command",
        .callback = [this](){
          return Add::run(this);
        }
      },

      Handler{
        .aliases = {"set"},
        .flags = {}, //TODO: Add flags
        .subcommands = Set::handlers(this),
        .docs = "set sub command",
        .callback = [this](){
          //OptionsInit::Set(this);
          return Set::run(this);
        }
      },

      Handler{
        .aliases = {"search"},
        .flags = {}, //TODO: Add flags
        .subcommands = Search::handlers(this),
        .docs = "search sub command",
        .callback = [this](){
          return Search::run(this);
        },
        .requires_project = false
      },
      Handler{
        .aliases = {"list", "ls"},
        .flags = {}, //TODO: Add flags
        .subcommands = List::handlers(this),
        .docs = "list sub command",
        .callback = [this](){
          return List::run(this);
        },
        .requires_project = false
      },

      Handler{
        .aliases = {"remove", "rm"},
        .flags = {}, //TODO: Add flags
        .subcommands = Remove::handlers(this),
        .docs = "remove sub command",
        .callback = [this](){
          return Remove::run(this);
        }
      },

      Handler{
        .aliases = {"update"},
        .flags = {}, //TODO: Add flags
        .subcommands = Update::handlers(this),
        .docs = "update sub command",
        .callback = [this](){
          return Update::run(this);
        }
      },

      Handler{
        .aliases = {"clean"},
        .flags = {"-c,--cache"}, //TODO: Add flags
        .docs = "clean sub command",
        .callback = [this](){
          return Clean::run(this);
        }
      },

      Handler{
        .aliases = {"build"},
        .flags = {"-m,--mode","-t,--target","-j,--jobs"}, //TODO: Add flags
        .docs = "build sub command",
        .callback = [this](){
          return Build::run(this);
        }
      },
      Handler{
        .aliases = {"watch"},
        .flags = {}, //TODO: Add flags
        .docs = "watches the project for changes",
        .callback = [this](){
          return Watch::run(this);
        }
      },
    };


    bool found_alias = false;
    Utils::Error error;
    for(Handler& handler : commands){
      for(std::string& alias : handler.aliases){
        if(alias == command){
          if(handler.requires_project && !project_present){
            error << "Error: Project not found and is required for this command" << ENDL;
            return false;
          }
          found_alias = true;
          if(!handler.callback()){
            return false;
            // std::cout << termcolor::red << "Error: Could not run: " << handler.aliases[0] << termcolor::reset << ENDL;
          }
        }
      }
    }
    if(!found_alias){
      std::cout << "Error: Command not found: " << command << ENDL;
    }
    return true;

  }
  void renderFlags(std::vector<std::string> flags){
    std::cout << "  [";
    for(std::string flag : flags){
      std::cout << " " << termcolor::bold << termcolor::magenta << flag << termcolor::reset;
    }
    std::cout << " ]";
  }
  void renderPositionals(std::vector<std::string> positionals){
    for(std::string positional : positionals){
      std::cout << termcolor::bold <<  termcolor::green << " <" << positional << ">" << termcolor::reset;
    }
  }
  bool Interface::runCommand(std::string command, std::vector<Handler> &handlers){
    Utils::Error error;
    for(Handler handler : handlers){
      for(std::string alias : handler.aliases){
        if(alias == command){
          if(!handler.implemented){
            error << "Command not implemented: " << command;
            return false;
          }
          if(handler.requires_project && !project_present){
            error << "Error: Project not found and command: " << command << " requires a project" << ENDL;
            return false;
          }
          if(!handler.callback()){
            getHelpString(handler);
            return false;
          }else{
            return true;
          }
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
        renderPositionals(handler.positional_args);
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
