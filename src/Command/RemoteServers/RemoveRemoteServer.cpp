#include <Frate/Command/RemoteServers.hpp>
#include <fstream>

namespace Frate::Command::RemoteServers{

  bool remove(std::shared_ptr<Interface> inter){
    std::vector<RemoteServer> servers =  remoteServerData(inter);
    std::string name;
    getServerName(name);
    std::erase_if(servers, [&name](RemoteServer &server){
        return server.name == name;
        });
    std::vector<json> build_server_json;
    for (RemoteServer& build_server: servers){
      json build_server_json_tmp = {
        {"name", build_server.name},
        {"address", build_server.ip},
        {"port", build_server.port},
        {"username", build_server.username},
        {"authMethod", build_server.authMethod},
        {"password", build_server.password.value_or("")},
        {"key", build_server.key.value_or("")}
      }; 
      build_server_json.push_back(build_server_json_tmp);
    }
    std::ofstream file;
    file.open(std::string(std::getenv("HOME"))  + "/.config/frate/" + "build_server.json");
    file << build_server_json;
    return true;
  }
}
