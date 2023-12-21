#include <Frate/System.hpp>
#include "Frate/Utils/General.hpp"
#include "Frate/Utils/Logging.hpp"

namespace Frate::System {
  bool Capabilities::search(){

    std::string path_env = std::getenv("PATH");
#ifdef _WIN32
    const char delimiter = ';';
    const std::string extension = ".exe";
#else
    const char delimiter = ':';
    const std::string extension = "";
#endif
    
    std::vector<std::string> paths = Utils::split(path_env, delimiter);
    for(std::filesystem::path path : paths){
      for(auto& p : std::filesystem::directory_iterator(path)){
        if(p.is_regular_file()){
          std::string filename = p.path().filename().string();
#ifdef _WIN32
          filename = filename.substr(0, filename.find(".exe"));
#endif
          if(filename.find("gcc") != std::string::npos){
            Utils::info << "Found gcc: " << p.path() << std::endl;
            this->compilers[filename] = Capability();
            this->compilers[filename].path = p.path();
            this->compilers[filename].installed = true;
          }
        }
      }
    }


    return false;
  }

  Capabilities::Capabilities(){

  }

  Capability Capabilities::getLatestCompiler(std::string compiler){

    return Capability();
  }
  void from_json(const nlohmann::json &j, Capabilities& capabilities){
  }
  void to_json(nlohmann::json &j, const Capabilities& capabilities){
  }
}
