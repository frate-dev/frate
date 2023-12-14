#include <Frate/Command/RemoteServers.hpp>
#include <fstream>


namespace Frate::Command::RemoteServers{

  bool add(std::shared_ptr<Interface> inter){
    std::vector<RemoteServer> servers =  remoteServerData(inter);
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
    servers.push_back(build_server);

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
    file.open(build_servers);
    file << build_server_json;
    return true;
  }
}
