#pragma once
#include <Frate/RemoteServer.hpp>
#include <vector>
#include "Frate/System.hpp"
#include "Frate/System/Capabilities.hpp"

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
      Command::RemoteServer getBuildServer();

      friend void from_json(const nlohmann::json &json_obj, ConfigManager& config);
      friend void to_json(nlohmann::json &json_obj, const ConfigManager& config);
  };
}
