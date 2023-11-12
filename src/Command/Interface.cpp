#include "Command.hpp"
#include "cxxopts.hpp"
#include <memory>


namespace Command {

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
    if(command != "init"){
      if(!this->LoadPackageJson()){
      }
    }



    using namespace cxxopts;
    if (command == "init"){
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
    else if (command == "add"){
      OptionsInit::Add(this);
      if(!this->add()){
        std::cout << "Error: Could not add project" << ENDL;
      }

    }else if(command == "search"){
      OptionsInit::Search(this);
      if(!this->search()){
        std::cout << "Error: Could not search project" << ENDL;
      }
    }
    else if (command == "server"){
      OptionsInit::Server(this);
      this->server();
    }
    else if (command == "remove"){
      OptionsInit::Add(this);
      if(!this->remove()){
        std::cout << "Error: Could not remove project" << ENDL;
      }
    }
    else if (command == "modes"){
      OptionsInit::Modes(this);
      if(!this->modes()){
        std::cout << "Error: Could not manage modes" << ENDL;
      } 
    }
    else if (command == "mode"){
      OptionsInit::Mode(this);
      if(!this->mode()){
        std::cout << "Error: Could not manage mode" << ENDL;
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
}
