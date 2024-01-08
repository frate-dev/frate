#pragma once
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

namespace Frate::Command {
  const int SSH_DEFAULT_PORT = 22;

  class RemoteServer {
  public:
    RemoteServer(std::string name = "", std::string server_ip = "",
                 std::string username = "", std::string auth_method = "",
                 std::optional<std::string> password = std::nullopt,
                 std::optional<std::string> key = std::nullopt,
                 int port = SSH_DEFAULT_PORT)
        : name(std::move(name)), ip(std::move(server_ip)),
          username(std::move(username)), authMethod(std::move(auth_method)),
          password(std::move(password)), key(std::move(key)), port(port){};

    std::string name;
    std::string ip;
    std::string username;
    std::string authMethod;
    std::optional<std::string> password;
    std::optional<std::string> key;
    int port;
    friend void from_json(const nlohmann::json &json_obj,
                          RemoteServer &remote_server);
    friend void to_json(nlohmann::json &json_obj,
                        const RemoteServer &remote_server);
  };

} // namespace Frate::Command
