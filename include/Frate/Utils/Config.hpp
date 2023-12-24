#pragma once
#include <Frate/RemoteServer.hpp>
#include <vector>
#include "Frate/System.hpp"
namespace Frate::Config {
  //TODO: work in progress
  class ConfigManager {
    public:
      bool first_time_setup{false};
      std::string build_server{""};
      std::vector<Command::RemoteServer> build_servers;
      System::Capabilities capabilities;
      bool load();
      bool save();
      bool createNew();
      friend void from_json(const nlohmann::json &j, ConfigManager& config);
      friend void to_json(nlohmann::json &j, const ConfigManager& config);
  };
}
