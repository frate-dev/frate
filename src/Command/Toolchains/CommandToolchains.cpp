#include <Frate/Command/Toolchains.hpp>
#include <Frate/Utils/General.hpp>



namespace Command::Toolchains{
  json load(){
    std::filesystem::path  path = std::string(std::getenv("HOME")) + "/.config/" + "frate/" + "toolchains.json";
    if(!std::filesystem::exists(path)){
      json data = Utils::fetchJson("https://github.com/frate-dev/toolchains/releases/download/index-6/index.json");
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
  bool list(){
    json data = load();
    for(auto toolchain : data["toolchains"]){
      std::cout << toolchain["name"] << std::endl;
    }
    return true;
  }
  bool add(std::string toolchain, Interface* interface){
    json data = load();
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
  bool remove(std::string toolchain_name, Interface* interface){
    json data = load();

    for (std::string toolchainProject : interface->pro->toolchains){
      if(toolchainProject == toolchain_name){
        std::erase_if(interface->pro->toolchains, [&toolchain_name](std::string &toolchain){
            return toolchain == toolchain_name;
            });
      }
    }
    return true;
  }
}
