#include <stdint.h> 
#include "Frate/Utils/General.hpp"
#include "termcolor/termcolor.hpp"
#include <Frate/Command.hpp>
#include <fstream>
#include <iostream>

namespace Command {
  bool Interface::LoadProjectJson() {
    using nlohmann::json;
    std::fstream file;
    std::string file_name = "frate-project.json";
    std::cout << "Loading: " << (pro->project_path / file_name) << std::endl;
    try{
      file.open((pro->project_path / file_name).string());
    }catch(std::exception &e){
      Utils::Error error;
      error << e.what() << std::endl;
      error << "Error: Could not open: " << (pro->project_path / file_name) << std::endl;
      return false;
    }
    json data;
    try {
      json data = json::parse(file);
      pro->checkKeys(data);
      pro->fromJson(data);
      //Simplfied this fucking code
      std::cout << "loadedJson = "  << pro->toJson() << ENDL;
    } catch (json::exception &e) {
      Utils::Error error;
      error << e.what() << std::endl;
      error << "Error: Could not load: " << (pro->project_path / file_name) << std::endl;
      return false;
    }
    file.close();
    return true;
  };
} // namespace Command
