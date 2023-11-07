#include "Command.hpp"
#include <nlohmann/json.hpp>
#include "../Utils/General.hpp"

namespace Command{
  using nlohmann::json;
  std::string indexFileName = std::string(std::getenv("HOME")) + "/.local/cmaker/index.json";
  std::string indexUrl = "https://github.com/cmaker-dev/index/releases/latest/download/index.json";
  /*
   * If index is not in path then we create one
   * returns fetched index
   */
  json fetchIndex(){
    json index;
    if(std::filesystem::exists(indexFileName)){
      std::ifstream indexFile;
      indexFile = std::ifstream(indexFileName);
      index = json::parse(indexFile);
      std::cout << "index file exists" <<ENDL;
    }else{
      std::ofstream indexFile; 
      std::cout << "Creating a new index file" << ENDL;
      std::system("mkdir -p ~/.local/cmaker");
      std::system("touch ~/.local/cmaker/index.json");
      index = Utils::fetchJson(indexUrl);
      std::cout << "god dee index" << ENDL;
      indexFile = std::ofstream(indexFileName);
      indexFile << index.dump(2);
      std::cout << "index file created" <<ENDL;
    }

    return index;
  }
  /*
   * Very similar to the fetch index function but this one will always update the index
   * TODO: Add a check to see if the index is up to date
   */
  void updateIndex(){
    std::ofstream indexFile;
    try{
      //Checks if the index file exists
      indexFile = std::ofstream(indexFileName);
    }catch(std::exception e){
      std::cout << "Creating a new index file" << ENDL;
      std::system("mkdir -p ~/.local/cmaker");
      std::system("touch ~/.local/cmaker/index.json");
      std::ifstream indexFile(indexFileName);
    }

    json data = Utils::fetchJson(indexUrl);
    indexFile << data.dump(2);
  }
  Package getExactPackage(std::string package_name){
    json index = fetchIndex();
    for(auto package : index){
      if(package["name"] == package_name){
        Package p;
        p.name = package["name"];
        p.versions = package["versions"];
        return p;
      }
    }
    Package p;
    p.name = "";
    return p;
  }
}
