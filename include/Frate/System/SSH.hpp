#include <Frate/RemoteServer.hpp>

namespace Frate::Command {
  class SSHConnection {
    public:
      SSHConnection(RemoteServer  server)
        : server(std::move(server)) {}

      [[nodiscard]] std::string createRemoteCommand(const std::string& command) const {
        std::string ssh_command = "ssh -p " + std::to_string(server.port) + " " +
          server.username + "@" + server.ip + " ";
        return ssh_command + "'" + command + "'";
      }

      [[nodiscard]] std::string createRsyncCommand(const std::string& local_path, const std::string& remote_path) const {
        return "rsync -avh --exclude-from='.gitignore' --update -e '" + createRemoteCommand("") +
          "' --progress " + local_path + " " + remote_path;
      }

    private:
      RemoteServer server;
  };
}
