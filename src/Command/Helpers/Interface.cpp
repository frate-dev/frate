#include <CMaker/Command.hpp>
#include "cxxopts.hpp"
#include <initializer_list>
#include <memory>

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
    if(command != "new" && command != "n"){
      if(!this->LoadPackageJson()){
      }
    }


    using namespace cxxopts;
    
    if (command == "new" || command == "n"){
      std::cout << "got here" << ENDL;
      OptionsInit::Init(this);
      if(!this->init()){
        std::cout << "Error: Could not initialize project" << ENDL;
      }
    }

    else if (command == "run"){
      if(!this->run()){
        std::cout << "Error: Could not run project" << ENDL;
      }
    }

    else if (command == "help"){
      if(!this->help()){
        std::cout << "Error: Could not display help" << ENDL;
      }
    }

    else if (command == "ftp"){
      if(!this->ftp()){
        std::cout << "Error: Could not ftp project" << ENDL;
      }
    }

    else if(command == "add"){
      OptionsInit::Add(this);
      if(!this->add()){
        std::cout << "Error: Could not add project" << ENDL;
      }
    }

    else if(command == "search"){
      OptionsInit::Search(this);
      if(!this->search()){
        std::cout << "Error: Could not search project" << ENDL;
      }
    }

    else if(command == "list"){
      OptionsInit::List(this);
      if(!this->list()){
        std::cout << "Error: Could not list project" << ENDL;
      }
    }

    else if (command == "remove" || command == "rm"){
      OptionsInit::Remove(this);
      if(!this->remove()){
        std::cout << "Error: Could not remove project" << ENDL;
      }

    }

    else if (command == "watch"){
      OptionsInit::Watch(this);
      if(!this->watch()){
        std::cout << "Error: Could not watch project" << ENDL;
      }
    }

    else if (command == "update"){
      OptionsInit::Update(this);
      if(!this->update()){
        std::cout << "Error: Could not update project index" << ENDL;
      }
    }

    else if (command == "clean"){
      OptionsInit::Clean(this);
      if(!this->clean()){
        std::cout << "Error: Could not clean project" << ENDL;
      }
    }

    else{
      std::cout << "Invalid command try one of these" << ENDL;
      this->help();

    }

  }
  Interface::~Interface(){
  }

  } // namespace Command
