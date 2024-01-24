#include <string>
#include <nlohmann/json.hpp>
#include <Frate/FrateException.hpp>

namespace Frate::Project {
  class OverrideHashFailure : public FrateException {
  public:
    OverrideHashFailure(const std::string &message) : FrateException(message) {}
  };

  class Config;
  class Cache {
  private:
    std::string build_command{"cmake --build ."};
    std::string test_command{"ctest"};
    std::string run_command{"./bin/"};
    std::string override_change_hash;
    std::shared_ptr<Config> config;

  public:
    Cache(std::shared_ptr<Config> config) : config(config) {}



    // Getters
    std::string &getBuildCommand() { return build_command; }

    std::string &getTestCommand() { return test_command; }

    std::string &getRunCommand() { return run_command; }

    std::string &getOverrideChangeHash() { return override_change_hash; }

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

    void setOverrideChangeHash(const std::string &override_change_hash) {
      this->override_change_hash = override_change_hash;
    }

    void generateOverrideChangeHash();

    friend void from_json(const nlohmann::json &json_obj, Cache &cache_obj);

    friend void to_json(nlohmann::json &json_obj, const Cache &cache_obj);
  };
} // namespace Frate::Project
