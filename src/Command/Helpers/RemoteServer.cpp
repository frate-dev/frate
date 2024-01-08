#include <Frate/RemoteServer.hpp>
#include <Frate/Utils/Logging.hpp>
#include <Frate/Utils/Macros.hpp>

namespace Frate::Command {

  void from_json(const nlohmann::json &json_obj, RemoteServer &remote_server) {
    FROM_JSON_FIELD(remote_server, name);
    FROM_JSON_FIELD(remote_server, ip);
    FROM_JSON_FIELD(remote_server, username);
    FROM_JSON_FIELD(remote_server, authMethod);
    FROM_JSON_FIELD(remote_server, password);
    FROM_JSON_FIELD(remote_server, key);
    FROM_JSON_FIELD(remote_server, port);
  }

  void to_json(nlohmann::json &json_obj, const RemoteServer &remote_server) {
    TO_JSON_FIELD(remote_server, name);
    TO_JSON_FIELD(remote_server, ip);
    TO_JSON_FIELD(remote_server, username);
    TO_JSON_FIELD(remote_server, authMethod);
    json_obj["password"] = remote_server.password.value_or("");
    json_obj["key"] = remote_server.key.value_or("");
    TO_JSON_FIELD(remote_server, port);
  }
} // namespace Frate::Command
