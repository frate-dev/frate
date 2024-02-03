#include "Frate/Utils/Crypto/md5.hpp"
#include <Frate/Project/Local.hpp>
#include <Frate/Project/Config.hpp>
#include <Frate/Utils/Logging.hpp>
#include <Frate/Utils/Macros.hpp>
#include <openssl/md5.h>
#include <sstream>
#include <fstream>

namespace Frate::Project {

  void from_json(const nlohmann::json &json_obj, Local &cache_obj) {
    FROM_JSON_FIELD(cache_obj, build_command);
    FROM_JSON_FIELD(cache_obj, test_command);
    FROM_JSON_FIELD(cache_obj, run_command);
    FROM_JSON_FIELD(cache_obj, override_change_hash);
  }

  void to_json(nlohmann::json &json_obj, const Local &cache_obj) {
    TO_JSON_FIELD(cache_obj, build_command);
    TO_JSON_FIELD(cache_obj, test_command);
    TO_JSON_FIELD(cache_obj, run_command);
    TO_JSON_FIELD(cache_obj, override_change_hash);
  }
  void Local::create_local_file(){
    std::ofstream file(this->config->path / LOCAL_FILE_NAME);
    if (!file.is_open()) {
      throw LocalIOException("Could not create local file at: " + this->config->path.string());
    }

    nlohmann::json json_obj;
    to_json(json_obj, *this);

    file << json_obj.dump(2);
  }
  void Local::load(){

  
    if(local_file_loaded){
      return;
    }

    if(!std::filesystem::exists(this->config->path / LOCAL_FILE_NAME)){
      this->create_local_file();
      this->local_file_loaded = true;
      return;
    }

    std::ifstream file(this->config->path / LOCAL_FILE_NAME);
    if (!file.is_open()) {
      throw LocalIOException("Could not open local file at: " + this->config->path.string());
    }

    nlohmann::json json_obj;
    file >> json_obj;

    try {
      from_json(json_obj, *this);
    } catch (nlohmann::json::exception &e) {
      throw LocalIOException("Could not parse local file at: " + this->config->path.string());
    }

    this->local_file_loaded = true;

  }
  void Local::save(){

    if(!local_file_loaded){
      throw LocalIOException("Could not save the local file either because it was not loaded or it was not created");
    }

    nlohmann::json json_obj;
    to_json(json_obj, *this);
    
    std::ofstream file(this->config->path / LOCAL_FILE_NAME);

    if (!file.is_open()) {
      throw LocalIOException("Could not open local file at: " + this->config->path.string());
    }

    file << json_obj.dump(2);
  }


  void Local::generateOverrideChangeHash() {

    Utils::MD5Encoder encoder;

    std::stringstream entry_time;

    for (auto &dir_entry : std::filesystem::recursive_directory_iterator(
             this->config->path / "override")) {
        entry_time << dir_entry.last_write_time().time_since_epoch().count();
    }
    std::string entry_time_str = entry_time.str();
    this->override_change_hash = encoder.intake(entry_time_str).getDigest();
  }
} // namespace Frate::Project
