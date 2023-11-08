#include "Command.hpp"

namespace  Command{
  bool getModeName(Mode &mode){
    Prompt<std::string> *name = new Prompt<std::string>("Name: ");
    mode.name = name->Run();
    return true;
  }

  bool modesAdd(Interface* interface){
    std::cout << "Adding mode" << std::endl; 
    Mode mode;
    getModeName(mode);
    interface->pro->modes.push_back(mode);
    return true;
  }
  bool modesRemove(Interface* interface){
    std::cout << "Removing mode" << std::endl;
    Prompt<std::string> *name = new Prompt<std::string>("Name: ");
    name->Run();
    std::string mode_name = name->Get();
    std::erase_if(interface->pro->modes, [&mode_name](Mode &mode){
            return mode.name == mode_name;
    });
    return true;
  }
  bool modesList(Interface* interface){
    for(auto mode : interface->pro->modes){
      std::cout << mode.name << std::endl;
    }
    return true;
  }
  bool Interface::modes(){
    this->LoadPackageJson();
    if(!args->count("subcommand")){
      this->help();
      return false;
    }
    std::string subcommand = args->operator[]("subcommand").as<std::string>();
    if(subcommand == "add"){
      return modesAdd(this);
    }
    if(subcommand == "remove"){
      return modesRemove(this);
    }
    if (subcommand == "list"){
      return modesList(this);
    }
    return true;
  }
}
