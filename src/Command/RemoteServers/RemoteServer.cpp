#include <Frate/Command/RemoteServers.hpp>
#include <Frate/Project.hpp>
#include <Frate/Utils/CLI.hpp>
#include <Frate/Utils/General.hpp>
#include <fstream>

namespace Frate::Command::RemoteServers {
  using namespace Utils::CLI;

  bool options(std::shared_ptr<Interface> inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand"});
    inter->options->add_options()(
        "command",
        "Command to run",
        cxxopts::value<std::string>()->default_value("help"))(
        "subcommand", "Subcommand to run", cxxopts::value<std::string>())(
        "h,help", "Print usage");
    return inter->parse();
  }

  bool validateServerInput(std::string input) {
    if (input.empty()) {
      return false;
    }
    if (input.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRST"
                                "UVWXYZ01234567890_") != std::string::npos) {
      return false;
    }
    return true;
  }

  bool getServerName(std::string &name) {
    Prompt name_promp("Enter the name of the server: ");
    name_promp.setValidator(validateServerInput);
    name_promp.run();

    auto [valid, _name] = name_promp.get<std::string>();
    if (!valid) {
      return false;
    }
    name = _name;
    return true;
  }

  bool getServerAddress(std::string &address) {
    Prompt address_prompt("Enter the address of the server: ");
    address_prompt.run();
    auto [valid, _address] = address_prompt.get<std::string>();
    if (!valid) {
      return false;
    }
    address = _address;
    return true;
  }

  bool getServerPort(int &port) {
    Prompt port_prompt("Enter the port of the server: ");
    port_prompt.run();
    auto [valid, _port] = port_prompt.get<int>();
    if (!valid) {
      return false;
    }
    port = _port;
    return true;
  }

  bool getServerUsername(std::string &username) {
    Prompt username_prompt("Enter the username of the server: ");
    username_prompt.maxLength(32).run();
    auto [valid, _username] = username_prompt.get<std::string>();
    if (!valid) {
      return false;
    }
    username = _username;
    return true;
  }

  bool getServerAuthMethod(std::string &authMethod) {
    Prompt authMethod_promp(
        "Enter the authentication method of the server[pem/password]: ");
    authMethod_promp.run();
    auto [valid, _authMethod] = authMethod_promp.get<std::string>();
    if (!valid) {
      return false;
    }
    authMethod = _authMethod;
    return true;
  }

  bool getServerPassword(std::string &password) {
    Prompt password_prompt("Enter the password of the server: ");
    password_prompt.run();
    auto [valid, _password] = password_prompt.get<std::string>();
    if (!valid) {
      return false;
    }
    password = _password;
    return true;
  }

  bool getServerKey(std::string &key) {
    Prompt key_prompt("Enter path the ssh key for the server: ");
    key_prompt.run();
    auto [valid, _key] = key_prompt.get<std::string>();
    if (!valid) {
      return false;
    }
    key = _key;
    return true;
  }

} // namespace Frate::Command::RemoteServers
