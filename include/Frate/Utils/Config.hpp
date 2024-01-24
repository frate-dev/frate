#pragma once
#include "Frate/System/Capabilities.hpp"
#include <Frate/Template/Manager.hpp>
#include <Frate/RemoteServer.hpp>
#include <vector>

namespace Frate::Config {
  class ConfigManager {
  public:
    bool first_time_setup{false};
    std::string build_server;
    std::vector<Command::RemoteServer> build_servers;
    System::Capabilities capabilities;
    //  TemplateManager templates;

    bool load();
    bool save();
    bool createNew();
    Command::RemoteServer getBuildServer();

    friend void from_json(const nlohmann::json &json_obj,
                          ConfigManager &config);
    friend void to_json(nlohmann::json &json_obj, const ConfigManager &config);
  };
} // namespace Frate::Config
