#include <CMaker/Command.hpp>
namespace Command{
  bool modesList(Interface* interface){
    for(auto mode : interface->pro->modes){
      std::cout << mode.name << std::endl;
    }
    return true;
  }
}
