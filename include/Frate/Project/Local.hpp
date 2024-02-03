#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <Frate/FrateException.hpp>

namespace Frate::Project {

  const std::string LOCAL_FILE_NAME = ".frate-local.json";

  class OverrideHashException : public FrateException {
  public:
    OverrideHashException(const std::string &message) : FrateException(message) {}
  };

  class LocalIOException : public FrateException {
  public:
    LocalIOException(const std::string &message) : FrateException(message) {}
  };

  class Config;
  class Local {
  private:
    std::string build_command{"cmake -build ."};
    std::string test_command{"ctest"};
    std::string run_command{"./bin/"};
    int requested_jobs{1};
    int max_jobs{1};
    std::string override_change_hash{};
    std::shared_ptr<Config> config;
    void create_local_file();
    bool local_file_loaded = false;

  public:
    Local() = default;

    Local(std::shared_ptr<Config> config) : config(config) {}
    // Getters
    std::string &getBuildCommand() { return build_command; }

    std::string &getTestCommand() { return test_command; }

    std::string &getRunCommand() { return run_command; }

    std::string &getOverrideChangeHash() { return override_change_hash; }

    int &getRequestedJobs() { return requested_jobs; }

    int &getMaxJobs() { return max_jobs; }

    // Setters
    void setBuildCommand(const std::string &build_command) {
      this->build_command = build_command;
    }

    void setTestCommand(const std::string &test_command) {
      this->test_command = test_command;
    }

    void setRunCommand(const std::string &run_command) {
      this->run_command = run_command;
    }

    void generateOverrideChangeHash();

    //JSON
    friend void from_json(const nlohmann::json &json_obj, Local &cache_obj);

    friend void to_json(nlohmann::json &json_obj, const Local &cache_obj);

    //IO
    
    void load();

    void save();


  };
} // namespace Frate::Project
