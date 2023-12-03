#include <Frate/Command.hpp>
#include <nlohmann/json.hpp>
#include <Frate/Utils/General.hpp>
#include <fstream>

namespace Frate::Command{
  using nlohmann::json;
  using std::cout;
  using std::ifstream;
  using std::ofstream;
  using std::string;
  using std::filesystem::create_directories;
  std::string indexFileName =
    std::string(std::getenv("HOME")) + "/.local/frate/index.json";
  std::string indexUrl =
    "https://github.com/frate-dev/index/releases/latest/download/index.json";
  /*
   * If index is not in path then we create one
   * returns fetched index
   */
  json fetchIndex() {
    json index;
    if (std::filesystem::exists(indexFileName)) {

      ifstream indexFile;
      indexFile = std::ifstream(indexFileName);
      index = json::parse(indexFile);
      std::cout << "index file exists" << ENDL;

    } else {
      ofstream indexFile;
      cout << "Creating a new index file" << ENDL;

      try {
        create_directories(string(std::getenv("HOME")) + "/.local/frate");
      }catch(std::exception& e){
        Frate::error << "Failed to create index file" << ENDL;
        Utils::debug(e.what());
        exit(-1);
      }

      index = Utils::fetchJson(indexUrl);
      indexFile = std::ofstream(indexFileName);
      indexFile << index.dump(2);
      std::cout << "index file created" <<ENDL;

    }

    return index;
  }
  /*
   * Very similar to the fetch index function but this one will always update the index
   * TODO: Add a check to see if the index is up to date
   * TODO: Add this to the interface caching the index in memory
   */
  bool updateIndex(){
    std::ofstream indexFile;

    try{

      //Checks if the index file exists
      indexFile = std::ofstream(indexFileName);

    }catch(std::exception& e){

      std::cout << "Creating a new index file" << ENDL;
      Utils::debug(e.what());

      try{

        create_directories(string(std::getenv("HOME")) + "/.local/frate");

      }catch(std::exception& e){
        Frate::error << "Failed to create index file" << ENDL;
        Utils::debug(e.what());
        return false;

      }

      std::ifstream indexFile(indexFileName);
    }

    json data = Utils::fetchJson(indexUrl);
    indexFile << data.dump(2);
    return true;
  }
  std::pair<bool,Package> getExactPackage(std::string package_name){

    json index = fetchIndex();

    for(auto package : index){

      if(package["name"] == package_name){

        Package p;
        p.name = package["name"];
        p.versions = package["versions"];
        p.git = package["git"];
        p.description = package["description"];
        return std::pair<bool,Package>{true,p};

      }
    }
    return std::pair<bool,Package>{false,Package{}};
  }
}
