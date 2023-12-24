#include <Frate/RemoteServer.hpp>
#include <Frate/Utils/Macros.hpp>
#include <Frate/Utils/Logging.hpp>


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
  void from_json(const nlohmann::json &j, RemoteServer& remoteServer){
    FROM_JSON_FIELD(remoteServer, name);
    FROM_JSON_FIELD(remoteServer, ip);
    FROM_JSON_FIELD(remoteServer, username);
    FROM_JSON_FIELD(remoteServer, authMethod);
    FROM_JSON_FIELD(remoteServer, password);
    FROM_JSON_FIELD(remoteServer, key);
    FROM_JSON_FIELD(remoteServer, port);
  }
  void to_json(nlohmann::json &j, const RemoteServer& remoteServer){
    TO_JSON_FIELD(remoteServer, name);
    TO_JSON_FIELD(remoteServer, ip);
    TO_JSON_FIELD(remoteServer, username);
    TO_JSON_FIELD(remoteServer, authMethod);
    j["password"] = remoteServer.password.value_or("");
    j["key"] = remoteServer.key.value_or("");
    TO_JSON_FIELD(remoteServer, port);
  }
}
