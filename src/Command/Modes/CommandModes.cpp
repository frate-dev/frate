#include <Frate/Command.hpp>
#include <Frate/Generators.hpp>
//#include <sys/socket.h>


namespace  Command::Modes{
  bool getModeName(Mode &mode){
    Prompt<std::string> *name = new Prompt<std::string>("Name: ");
    name->Run();
    mode.name = name->Get();
    return true;
  }
  bool add(Interface* inter){
    std::cout << "Adding mode" << std::endl; 
    Mode mode;
    getModeName(mode);
    inter->pro->modes.push_back(mode);

    std::cout << "Writing config.json" << std::endl;
    if(!Generators::ConfigJson::writeConfig(inter->pro)){
      std::cout << "Failed to write config.json" << std::endl;
    }

    if(!Generators::CMakeList::createCMakeListsExecutable(inter->pro)){
      std::cout << "Failed to write CMakeLists.txt" << std::endl;
    }
    return true;
  }
  bool remove(Interface* inter){
    std::cout << "Removing mode" << std::endl;
    Prompt<std::string> *name = new Prompt<std::string>("Name: ");
    name->Run();
    std::string mode_name = name->Get();

    std::erase_if(inter->pro->modes, [&mode_name](Mode &mode){
        return mode.name == mode_name;
        });
    return true;
  }
  bool list(Interface* inter){
    for(auto mode : inter->pro->modes){
      std::cout << mode.name << std::endl;
    }
    return true;
  }
}
