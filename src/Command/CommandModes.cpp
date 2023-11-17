#include "Command.hpp"
#include "../Generators/Generators.hpp"
#include <sys/socket.h>

namespace  Command{


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

}
