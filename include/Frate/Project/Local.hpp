#pragma once
#include <Frate/FrateException.hpp>
#include <nlohmann/json.hpp>
#include <string>

namespace Frate::Project {

  const std::string LOCAL_FILE_NAME = ".frate-local.json";

  class OverrideHashException : public FrateException {
  public:
    OverrideHashException(const std::string &message)
        : FrateException(message) {}
  };

  class LocalIOException : public FrateException {
  public:
    LocalIOException(const std::string &message) : FrateException(message) {}
  };

  class Config;

  class Local {
  private:
    std::string build_command;
    std::string test_command;
    std::string run_command;
    std::string build_mode;
    int requested_jobs{1};
    int max_jobs{1};
    std::string override_change_hash{};
    std::shared_ptr<Config> project;
    bool local_file_loaded = false;
    void create_local_file();
    void gen_default_build_command();
    void gen_default_test_command();
    void gen_default_run_command();

  public:
    Local() = default;

    Local(std::shared_ptr<Config> config) : project(config) {}

    // Getters
    std::string &getBuildCommand();
    std::string &getTestCommand();
    std::string &getRunCommand();

    std::string &getOverrideChangeHash() { return override_change_hash; }

    int &getRequestedJobs() { return requested_jobs; }

    int &getMaxJobs() { return max_jobs; }

    std::string &getBuildMode() { return build_mode; }
    
    std::string getProjectPath();

    // Setters
    void setBuildCommand(std::string build_command) {
      this->build_command = build_command;
    }

    void setTestCommand(std::string test_command) {
      this->test_command = test_command;
    }

    void setRunCommand(std::string run_command) {
      this->run_command = run_command;
    }

    void setRequestedJobs(int requested_jobs) {
      this->requested_jobs = requested_jobs;
    }

    void setBuildMode(std::string build_mode) { this->build_mode = build_mode; }

    void generateOverrideChangeHash();

    // JSON
    friend void from_json(const nlohmann::json &json_obj, Local &cache_obj);

    friend void to_json(nlohmann::json &json_obj, const Local &cache_obj);

    // IO

    void load();

    void save();
  };
} // namespace Frate::Project
