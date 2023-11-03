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

    this->ctx = std::make_shared<Context>();
    OptionsInit::Main(this);
    this->parse();
    //After the parse we can set the context args
    this->ctx->args = this->args;

    // if(!this->args->count("command")){
    //   this->help();
    // }

#ifdef DEBUG
      ctx->project_path = std::filesystem::current_path() / "build";
#else
      ctx->project_path = std::filesystem::current_path();
#endif

    std::string command = this->args->operator[]("command").as<std::string>();

    #ifdef DEBUG
        std::cout << "DEBUG MODE ENABLED\n";
    #endif
    if(command != "init"){
      this->LoadPackageToml();
    }


    using namespace cxxopts;
    if (command == "init"){
      OptionsInit::Init(this);
      this->init();
    }
    else if (command == "run"){
      this->run();
    }
    else if (command == "help"){
      this->help();
    }

    else if (command == "ftp"){
      this->ftp();
    }
    else if (command == "add"){
      OptionsInit::Add(this);
      this->add();

    }
    else if (command == "remove"){
      OptionsInit::Add(this);
      this->remove();
    }
    else if (command == "watch"){
      OptionsInit::Watch(this);
      this->watch();
    }
    else if (command == "update"){
      OptionsInit::Update(this);
      this->update();
    }
    else if (command == "clean"){
      this->clean();
    }
    else{
      std::cout << "Invalid command try one of these" << ENDL;
      this->help();

    }
  }
  Interface::~Interface(){
  }
}
