#pragma once
#include <string>
#include <optional>

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
  };
}
