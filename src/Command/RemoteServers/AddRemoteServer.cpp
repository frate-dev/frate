#include <Frate/Command/RemoteServers.hpp>
#include <nlohmann/json.hpp>


namespace Frate::Command::RemoteServers{
  using nlohmann::json;

  bool add(std::shared_ptr<Interface> inter){
    // std::vector<RemoteServer> servers = remoteServerData(inter);
    std::string build_servers= std::string(std::getenv("HOME"))  + "/.config/frate/" + "build_server.json";
    std::string name, address,  username, authMethod, password, key;
    int port;
    getServerName(name);
    getServerAddress(address);
    getServerPort(port);
    getServerUsername(username);
    getServerAuthMethod(authMethod);
    if (authMethod == "password") {
      getServerPassword(password);
    }
    else if (authMethod == "pem") {
      getServerKey(key);
    }
    else{
      std::cout << "Invalid authentication method" << std::endl;
      return false;
    }

    RemoteServer build_server = RemoteServer(name, address, username, authMethod, password, key,  port);

    inter->config.build_servers.push_back(build_server);

    return true;
  }
}
