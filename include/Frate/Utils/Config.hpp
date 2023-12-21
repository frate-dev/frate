#pragma once
#include <Frate/RemoteServer.hpp>
#include <vector>
#include "Frate/System.hpp"
namespace Frate::Config {
  //TODO: work in progress
  class ConfigManager {
    public:
      bool first_time_setup{false};
      std::string current_remote_server{""};
      std::vector<Command::RemoteServer> servers;
      System::Capabilities capabilities;
      friend void from_json(const nlohmann::json &j, ConfigManager& config);
      friend void to_json(nlohmann::json &j, const ConfigManager& config);
  };
}
