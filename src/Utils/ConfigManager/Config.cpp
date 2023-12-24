#include <Frate/Utils/Config.hpp>
#include <Frate/Utils/Macros.hpp>
#include <Frate/Utils/Logging.hpp>
#include <Frate/Constants.hpp>
#include <fstream>
namespace Frate::Config {

  void from_json(const nlohmann::json& j, ConfigManager& config) {
    FROM_JSON_FIELD(config, capabilities);
    FROM_JSON_FIELD(config, first_time_setup);
    FROM_JSON_FIELD(config, build_server);
    FROM_JSON_FIELD(config, build_servers);
  }

  void to_json(nlohmann::json &j, const ConfigManager& config){
    TO_JSON_FIELD(config, capabilities);
    TO_JSON_FIELD(config, first_time_setup);
    TO_JSON_FIELD(config, build_server);
    TO_JSON_FIELD(config, build_servers);
  }
  
  Command::RemoteServer ConfigManager::getBuildServer(){
    if(this->build_server.empty()){
      Utils::error << "No build server set" << std::endl;
      exit(1);
    }
    for(auto &server : this->build_servers){
      if(server.name == this->build_server){
        return server;
      }
    }
    Utils::error << "Build server not found" << std::endl;
    exit(1);
  }

  bool ConfigManager::createNew(){
    //Creating config directory
    try{

      std::filesystem::create_directories(Constants::CONFIG_DIR);

    } catch(std::exception& e){

      Utils::error << "Failed to create new config directory: " << e.what() << std::endl;
      return false;
    }

    // Create config file
    std::ofstream config_file;
    try{

      config_file.open(Constants::CONFIG_PATH);

    } catch(std::exception& e){

      Utils::error << "Failed to create new config file: " << e.what() << std::endl;
      return false;
    }
    try{

      nlohmann::json config_json = *this;
      config_file << config_json.dump(2);

    } catch(std::exception& e){
      Utils::error << "Failed to write to new config file: " << e.what() << std::endl;
      return false;
    }

    return true;
  }
  bool ConfigManager::load(){

    // Creates config file if it does not exist
    if(!std::filesystem::exists(Constants::CONFIG_PATH)){
      Utils::info << "Config file does not exist, creating..." << std::endl;
      if(!createNew()){
        Utils::error << "Failed to create new config file" << std::endl;
        return false;
      }
    }

    // Now we attempt to open the new config file which should exist

    std::ifstream config_file;

    try{

      config_file.open(Constants::CONFIG_PATH); 

    } catch(std::exception& e){

      Utils::error << "Failed to open config file: " << e.what() << std::endl;
      return false;
    }
    
    try{

      nlohmann::json config_json = nlohmann::json::parse(config_file);
      from_json(config_json, *this);

    } catch(std::exception& e){

      Utils::error << "Failed to parse config file: " << e.what() << std::endl;
      return false;
    }
    return true;
  }  // namespace Frate::Config
  bool ConfigManager::save(){
    if(!std::filesystem::exists(Constants::CONFIG_PATH)){
      Utils::info << "Config file does not exist, creating..." << std::endl;
      if(!createNew()){
        Utils::error << "Failed to create new config file" << std::endl;
        return false;
      }
    }


    std::ofstream config_file;
    try{
      config_file.open(Constants::CONFIG_PATH);
      nlohmann::json config_json = *this;
      config_file << config_json.dump(2);
    } catch(std::exception& e){
      Utils::error << "Failed to open config file: " << e.what() << std::endl;
      return false;
    }
    return true;
  }
}
