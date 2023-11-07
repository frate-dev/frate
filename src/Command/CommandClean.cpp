#include "Command.hpp"
#include <filesystem>


namespace Command{
  bool Interface::clean(){
    std::cout << "Cleaning: " << std::endl;
    std::cout << pro->project_path / "build/*" << std::endl;
    for(auto& p: std::filesystem::directory_iterator(pro->project_path / "build")){
      std::cout << p.path() << std::endl;
    }
    return true;
  }
}
