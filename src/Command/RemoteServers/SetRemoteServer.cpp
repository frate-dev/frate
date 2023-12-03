#include <Frate/Command/RemoteServers.hpp>



namespace Frate::Command::RemoteServers{
  bool set(Interface* inter){
    std::vector<RemoteServer> servers =  remoteServerData(inter);
    std::string name;
    getServerName(name);
    for (auto& server: servers){
      std::cout << "server.name:" << server.name << std::endl;
      std::cout << "name:" << name << std::endl;
      if (server.name == name){

        std::cout << "Found server" << std::endl;
        std::ofstream file;
        file.open(std::string(std::getenv("HOME"))  + "/.config/frate/" + "current_build_server.json");
        json current_build_server = {
          {"name", server.name},
          {"address", server.ip},
          {"port", server.port},
          {"username", server.username},
          {"authMethod", server.authMethod},
          {"password", server.password.value_or("")},
          {"key", server.key.value_or("")}
        };
        file << current_build_server;
        return true;
      }
    }
    std::cout << "Could not find server" << std::endl;
    return false;
  }
}
