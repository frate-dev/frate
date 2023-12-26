#include <Frate/Command/RemoteServers.hpp>
#include <nlohmann/json.hpp>

namespace Frate::Command::RemoteServers {
  using nlohmann::json;

  bool remove(std::shared_ptr<Interface> inter) {
    // std::vector<RemoteServer> servers =  remoteServerData(inter);
    std::string name;
    getServerName(name);
    std::erase_if(inter->config.build_servers, [&name](RemoteServer &server) {
      return server.name == name;
    });
    return true;
  }
} // namespace Frate::Command::RemoteServers
