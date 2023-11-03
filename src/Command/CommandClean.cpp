#include "Command.hpp"
#include <filesystem>


namespace Command{
  bool Interface::clean(){
    std::cout << "Cleaning: " << std::endl;
    std::cout << ctx->project_path / "build/*" << std::endl;
    for(auto& p: std::filesystem::directory_iterator(ctx->project_path / "build")){
      std::cout << p.path() << std::endl;
    }
    return true;
  }
}
