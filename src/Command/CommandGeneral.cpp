#include "Frate/Utils/General.hpp"
#include <Frate/Command.hpp>
#include <fstream>
#include <iostream>

namespace Frate::Command {
  bool Interface::LoadProjectJson() {
    using nlohmann::json;
    std::fstream file;
    std::string file_name = "frate-project.json";

    Frate::info << "Loading: " << (pro->project_path / file_name) << std::endl;

    if(!std::filesystem::exists(pro->project_path / file_name)){
      return false;
    }


    try{
      file.open((pro->project_path / file_name).string());
    }catch(std::exception &e){
      Utils::debug(e.what());
      return false;
    }

    try{
      json j = json::parse(file);
      pro->fromJson(j);
    }catch(std::exception &e){
      Utils::debug(e.what());
      return false;
    }

    return true;
  };
} // namespace Command
