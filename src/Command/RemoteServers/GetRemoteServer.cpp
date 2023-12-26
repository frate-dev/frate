#include <Frate/Command/RemoteServers.hpp>
#include <Frate/Project.hpp>
#include <Frate/Utils/General.hpp>
#include <fstream>

namespace Frate::Command::RemoteServers {

  bool get(std::shared_ptr<Interface> inter) {
    Utils::TableFormat table;
    table.width = 20;
    table << "Name"
          << "Address"
          << "Port"
          << "Username"
          << "AuthMethod"
          << "\n";
    table << inter->pro->build_server.name << inter->pro->build_server.ip
          << inter->pro->build_server.port << inter->pro->build_server.username
          << inter->pro->build_server.authMethod << "\n";
    return true;
  }

  std::optional<json> parseJsonFile(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file) {
      std::cerr << "Unable to open file: " << filePath << std::endl;
      return std::nullopt;
    }

    try {
      json j = json::parse(file);
      return j;
    } catch (const json::parse_error &e) {
      std::cerr << "JSON parse error in file " << filePath << ": " << e.what()
                << std::endl;
      return std::nullopt;
    }
  }

  RemoteServer get_current_build_server() {
    std::string current_build_server = std::string(std::getenv("HOME")) +
                                       "/.config/frate/" +
                                       "current_build_server.json";
    std::optional<json> data = parseJsonFile(current_build_server);
    if (!data.has_value()) {
      std::cerr << "No current build server found" << std::endl;
      std::cerr << "Please run frate add server to add a server" << std::endl;
      std::cerr << "Please run frate set server to set a default a server"
                << std::endl;
      return RemoteServer();
    }
    json current_build_server_json = data.value();
    if (!current_build_server_json["name"].is_null()) {
      return RemoteServer(
          current_build_server_json["name"].get<std::string>(),
          current_build_server_json["address"].get<std::string>(),
          current_build_server_json["username"].get<std::string>(),
          current_build_server_json["authMethod"].get<std::string>(),
          current_build_server_json["password"].get<std::string>(),
          current_build_server_json["key"].get<std::string>(),
          current_build_server_json["port"].get<int>());
    }
    return RemoteServer();
  }
} // namespace Frate::Command::RemoteServers
