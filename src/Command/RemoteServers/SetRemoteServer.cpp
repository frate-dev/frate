#include <Frate/Command/RemoteServers.hpp>
#include <nlohmann/json.hpp>

namespace Frate::Command::RemoteServers {
  using nlohmann::json;

  bool set(std::shared_ptr<Interface> inter) {
    // std::vector<RemoteServer> servers =  remoteServerData(inter);
    std::string name;
    getServerName(name);
    for (auto &server : inter->config.build_servers) {
      std::cout << "server.name:" << server.name << std::endl;
      std::cout << "name:" << name << std::endl;
      if (server.name == name) {
        inter->config.build_server = server.name;
        return true;
      }
    }
    std::cout << "Could not find server" << std::endl;
    return false;
  }
} // namespace Frate::Command::RemoteServers
