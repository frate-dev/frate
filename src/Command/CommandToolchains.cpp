#include "Command.hpp"
#include "../Utils/General.hpp"



namespace Command{
  json loadToolchains(){
    std::filesystem::path  path = std::string(std::getenv("HOME")) + "/.config/" + "cmaker/" + "toolchains.json";
    if(!std::filesystem::exists(path)){
      json data = Utils::fetchJson("https://github.com/cmaker-dev/toolchains/releases/download/index-6/index.json");
      if(data == nullptr){
        std::cout << "error: failed to fetch toolchains" << std::endl;
        return false;
      } 
      std::ofstream file(path);
      file << data.dump(2);
    }
    std::ifstream file(path);
    json data = json::parse(file);
    std::cout << "loaded toolchains" << std::endl;
    return data;
  }
  bool listToolchains(){
    json data = loadToolchains();
    for(auto toolchain : data["toolchains"]){
      std::cout << toolchain["name"] << std::endl;
    }
    return true;
  }
  bool addToolchain(std::string toolchain, Interface* interface){
    json data = loadToolchains();
    for(auto toolchainJson : data["toolchains"]){
      if(toolchainJson["name"] == toolchain){
        std::cout << "found toolchain" << std::endl;
        std::cout << toolchainJson.dump(2) << std::endl;
        interface->pro->toolchains.push_back(toolchain);
        return true;
      }
    }

    return true;
  }
  bool removeToolchain(std::string user_toolchain, Interface* interface){
    json data = loadToolchains();

    for (std::string toolchainProject : interface->pro->toolchains){
      if(toolchainProject == user_toolchain){
        std::erase_if(interface->pro->toolchains, [&user_toolchain](std::string &toolchain){
                return toolchain == user_toolchain;
        });
      }
    }


    return true;
  }
  bool  Interface::toolchains(){
    std::string command = args->operator[]("command").as<std::string>();
    if (command == "list"){
      listToolchains();
      return true;
    }
    if (command == "add"){
      addToolchain(args->operator[]("arguments").as<std::string>(), this);
      return true;
    }
    if (command == "remove"){
      removeToolchain(args->operator[]("arguments").as<std::string>(), this);
      return true;
    }

    return true;
  }
}
