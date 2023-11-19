#include <CMaker/Command.hpp>
#include "cxxopts.hpp"
#include "termcolor/termcolor.hpp"
#include <initializer_list>
#include <memory>
#include <sstream>

namespace Command {
using std::function;
using std::initializer_list;

bool OptionsInit::Main(Interface *inter) {
  inter->InitHeader();
  inter->options->parse_positional({"command"});
  inter->options->allow_unrecognised_options().add_options()(
      "command", "Command to run",
      cxxopts::value<std::string>()->default_value("help"))(
      "v,verbose", "Verbose output",
      cxxopts::value<bool>()->default_value("false"))(
      "y,confim-all", "skip all y/n prompts",
      cxxopts::value<bool>()->default_value("false"))("h,help", "Print usage");
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

  // bool Interface::registerCommand(initializer_list<std::string> aliases,
  //                                 initializer_list<std::string> flags,
  //                                 std::string docs,
  //                                 function<bool()> func) {
  //   commands.push_back(Handler(aliases, flags,docs, func));
  //   return true;
  // }
  bool Interface::InitHeader(){
    try{
      this->options = std::make_shared<cxxopts::Options>("CMaker", "A CMake project generator, we suffer so you don't have to!");
    } catch (std::exception& e) {
      std::cout << e.what() << std::endl;
      return false;
    }
    return true;
  }

  Interface::Interface(int argc, char** argv){
    this->argc = argc;
    this->argv = argv;

    this->pro = std::make_shared<Project>();
    OptionsInit::Main(this);
    this->parse();
    //After the parse we can set the context args
    this->pro->args = this->args;

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

    #ifdef DEBUG
        std::cout << "DEBUG MODE ENABLED\n";
    #endif
    #ifdef DEBUG
        pro->project_path = std::filesystem::current_path() / "build";
    #else
        pro->project_path = std::filesystem::current_path();
    #endif

    std::cout << "Project Path: " << pro->project_path << ENDL;
    commands = {

      Handler{
        .aliases = {"new", "n"},
        .flags = {}, //TODO: Add flags
        .docs = "Create a new project",
        .callback = [this](){
          OptionsInit::Init(this);
          return this->init();
        }
      },

      Handler{
        .aliases = {"run"},
        .flags = {}, //TODO: Add flags
        .docs = "Run the project",
        .callback = [this](){
          return this->run();
        }
      },

      Handler{
        .aliases = {"help", "h"},
        .flags = {}, //TODO: Add flags
        .docs = "Display help",
        .callback = [this](){
          return this->help();
        }
      },

      Handler{
        .aliases = {"ftp"},
        .flags = {}, //TODO: Add flags
        .docs = "Deletes the entire project F*ck This Project",
        .callback = [this](){
          return this->ftp();
        }
      },

      Handler{
        .aliases = {"add"},
        .flags = {}, //TODO: Add flags
        .subcommands = getAddHandlers(),
        .docs = "add sub command",
        .callback = [this](){
          OptionsInit::Add(this);
          return this->add();
        }
      },

      Handler{
        .aliases = {"search"},
        .flags = {}, //TODO: Add flags
        .docs = "search sub command",
        .callback = [this](){
          OptionsInit::Search(this);
          return this->search();
        }
      },

      Handler{
        .aliases = {"list", "ls"},
        .flags = {}, //TODO: Add flags
        .subcommands = getListHandlers(),
        .docs = "list sub command",
        .callback = [this](){
          OptionsInit::List(this);
          return this->list();
        }
      },

      Handler{
        .aliases = {"remove", "rm"},
        .flags = {}, //TODO: Add flags
        .docs = "remove sub command",
        .callback = [this](){
          OptionsInit::Remove(this);
          return this->remove();
        }
      },

      Handler{
        .aliases = {"update"},
        .flags = {}, //TODO: Add flags
        .docs = "update sub command",
        .callback = [this](){
          OptionsInit::Update(this);
          return this->update();
        }
      },

      Handler{
        .aliases = {"clean"},
        .flags = {}, //TODO: Add flags
        .docs = "clean sub command",
        .callback = [this](){
          OptionsInit::Clean(this);
          return this->clean();
        }
      },

      Handler{
        .aliases = {"build"},
        .flags = {}, //TODO: Add flags
        .docs = "build sub command",
        .callback = [this](){
          OptionsInit::Build(this);
          return this->build();
        }
      },
      Handler{
        .aliases = {"watch"},
        .flags = {}, //TODO: Add flags
        .docs = "watches the project for changes",
        .callback = [this](){
          OptionsInit::Watch(this);
          return this->watch();
        }
      },
    };

    bool found_alias = false;
    for(Handler& handler : commands){
      for(std::string& alias : handler.aliases){
        if(alias == command){
          found_alias = true;
          if(!handler.callback()){
            std::cout << "Error: Could not run: " << handler.aliases[0] << ENDL;
          }
        }
      }
    }
    if(!found_alias){
      std::cout << "Error: Command not found: " << command << ENDL;
    }

  }
  void Interface::getHelpString(std::string name,std::vector<Handler> &handlers, bool is_subcommand){
    int index = 0;
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
        if(alias != handler.aliases.back()){
          alias_str_len += 1;
          std::cout << " | ";
        }
      }
      if(is_subcommand){
        std::cout << " ";
        if(index != handlers.size()){
          std::cout << "\n   │";
        }else{
          std::cout << "\n    ";
        }
        std::cout << "    └" << termcolor::blue << handler.docs << termcolor::reset << ENDL;
      }else{
        if(handler.subcommands.size() > 0){
          std::cout << termcolor::blue << " <target>" << termcolor::reset << ENDL;

        }else{
          std::cout << " : " << termcolor::blue << handler.docs << termcolor::reset << ENDL;
        }
      }
      if(handler.subcommands.size() > 0){
        getHelpString(name + " " + handler.aliases[0], handler.subcommands, true);
        std::cout << ENDL;
      }
    }
  }
  Interface::~Interface(){
  }

  } // namespace Command
