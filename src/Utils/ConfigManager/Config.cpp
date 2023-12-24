#include <Frate/Utils/Config.hpp>
#include <Frate/Utils/Macros.hpp>
#include <Frate/Utils/Logging.hpp>
#include <Frate/Constants.hpp>
#include <fstream>
namespace Frate::Config {

  void from_json(const nlohmann::json& j, ConfigManager& config) {
    FROM_JSON_FIELD(config, capabilities);
    FROM_JSON_FIELD(config, first_time_setup);
    FROM_JSON_FIELD(config, current_remote_server);
    FROM_JSON_FIELD(config, servers);
  }
  void to_json(nlohmann::json &j, const ConfigManager& config){
    TO_JSON_FIELD(config, capabilities);
    TO_JSON_FIELD(config, first_time_setup);
    TO_JSON_FIELD(config, current_remote_server);
    TO_JSON_FIELD(config, servers);
  }
  bool ConfigManager::load(){
    std::ifstream config_file;
    try{
      config_file.open(Constants::CONFIG_PATH);
    } catch(std::exception& e){
      Utils::error << "Failed to open config file: " << e.what() << std::endl;
      return false;
    }
    return true;
  }  // namespace Frate::Config
}
