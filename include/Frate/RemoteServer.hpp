#pragma once
#include <string>
#include <optional>
#include <nlohmann/json.hpp>


namespace Frate::Command {
  class RemoteServer{
    public:
      RemoteServer(
        std::string name="",
        std::string ip="",
        std::string username="",
        std::string authMethod="",
        std::optional<std::string> password=std::nullopt,
        std::optional<std::string> key=std::nullopt,
        int port=22
      );
      std::string name;
      std::string ip;
      std::string username;
      std::string authMethod;
      std::optional<std::string> password;
      std::optional<std::string> key;
      int port;
      friend void from_json(const nlohmann::json &json_obj, RemoteServer& remoteServer);
      friend void to_json(nlohmann::json &json_obj, const RemoteServer& remoteServer);
  };

}
