#include <Frate/RemoteServer.hpp>


namespace Frate::Command {
  RemoteServer::RemoteServer(
      std::string name,
      std::string ip,
      std::string username,
      std::string authMethod,
      std::optional<std::string> password,
      std::optional<std::string> key,
      int port
      ){
    this->name = name;
    this->ip = ip;
    this->username = username;
    this->authMethod = authMethod;
    this->password = password;
    this->key = key;
    this->port = port;
  }
}
