#include <Frate/Command/RemoteServers.hpp>
#include <Frate/Utils/General.hpp>
#include <Frate/Utils/CLI.hpp>
#include <fstream>
#include <Frate/Project.hpp>

namespace Frate::Command::RemoteServers{
  using namespace Utils::CLI;
  bool options(std::shared_ptr<Interface> inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command", "subcommand"});
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("subcommand", "Subcommand to run", cxxopts::value<std::string>())("h,help", "Print usage");
    return inter->parse();
  }

  std::vector<RemoteServer> remoteServerData(std::shared_ptr<Interface> inter){
    std::fstream file;
    std::string build_servers_dir= std::string(std::getenv("HOME"))  + "/.config/frate/";
    if (!std::filesystem::exists(build_servers_dir)){
      try{
        std::filesystem::create_directory(build_servers_dir);
      }catch(std::exception &e){
        Utils::debug(e.what());
        Utils::error <<
          "Error while creating directory: " << build_servers_dir << std::endl;
        exit(1);
      }
    }
    std::string build_servers= std::string(std::getenv("HOME"))  + "/.config/frate/" + "build_server.json";
    std::string current_build_server= std::string(std::getenv("HOME"))  + "/.config/frate/" + "current_build_server.json";
    if (!std::filesystem::exists(build_servers)){
      std::ofstream file(build_servers);
      file << "[]";
    }
    if (!std::filesystem::exists(current_build_server)){
      std::ofstream file(current_build_server);
      file << "{}";
    }
    file.open(build_servers);

    std::vector<RemoteServer> servers;
    json server_list;
    try{
      server_list = json::parse(file);
    }
    catch(json::exception &e){
      std::cout << "Error: Could not load build_server.json" << std::endl;
      Utils::debug(e.what());
      exit(1);
    }
    try{
      json current_build_server_json = json::parse(std::ifstream(current_build_server));
      if (!current_build_server_json["name"].is_null()) {
        inter->pro->build_server = RemoteServer(
            current_build_server_json["name"].get<std::string>(),
            current_build_server_json["address"].get<std::string>(), 
            current_build_server_json["username"].get<std::string>(),
            current_build_server_json["authMethod"].get<std::string>(),
            current_build_server_json["password"].get<std::string>(),
            current_build_server_json["key"].get<std::string>(),
            current_build_server_json["port"].get<int>()
            );
      }

    }
    catch(json::exception &e){
      std::cout << "Error: Could not load current_build_server.json" << std::endl;
      Utils::debug(e.what());
      exit(1);
    }
    for (json& server : server_list){
      RemoteServer  build_server = RemoteServer(
          server["name"].get<std::string>(),
          server["address"].get<std::string>(), 
          server["username"].get<std::string>(),
          server["authMethod"].get<std::string>(),
          server["password"].get<std::string>(),
          server["key"].get<std::string>(),
          server["port"].get<int>()
          );
      servers.push_back(build_server);
    }
    return servers;
  }
  bool validateServerInput(std::string input){
    if(input.empty()){
      return false;
    }
    if (input.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos){
      return false;
    }
    return true;
  }

  bool getServerName(std::string& name){
    Prompt name_promp("Enter the name of the server: ");
    name_promp.setValidator(validateServerInput);
    name_promp.run();

    auto[valid, _name] = name_promp.get<std::string>();
    if(!valid){
      return false;
    }
    name = _name;
    return true;
  }
  bool getServerAddress(std::string& address){
    Prompt address_prompt("Enter the address of the server: ");
    address_prompt.run();
    auto [valid, _address] = address_prompt.get<std::string>();
    if(!valid){
      return false;
    }
    address = _address;
    return true;
  }

  bool getServerPort(int& port){
    Prompt port_prompt("Enter the port of the server: ");
    port_prompt.run();
    auto[valid,_port] = port_prompt.get<int>();
    if(!valid){
      return false;
    }
    port = _port;
    return true;
  }

  bool getServerUsername(std::string& username){
    Prompt username_prompt("Enter the username of the server: ");
    username_prompt.maxLength(32).run();
    auto[valid, _username] = username_prompt.get<std::string>();
    if(!valid){
      return false;
    }
    username = _username;
    return true;
  }

  bool getServerAuthMethod(std::string& authMethod){
    Prompt authMethod_promp("Enter the authentication method of the server[pem/password]: ");
    authMethod_promp.run();
    auto[valid,_authMethod] = authMethod_promp.get<std::string>();
    if(!valid){
      return false;
    }
    authMethod = _authMethod;
    return true;
  }

  bool getServerPassword(std::string& password){
    Prompt password_prompt("Enter the password of the server: ");
    password_prompt.run();
    auto[valid,_password] = password_prompt.get<std::string>();
    if(!valid){
      return false;
    }
    password = _password;
    return true;
  }

  bool getServerKey(std::string& key){
    Prompt key_prompt("Enter path the ssh key for the server: ");
    key_prompt.run();
    auto[valid,_key] = key_prompt.get<std::string>();
    if(!valid){
      return false;
    }
    key = _key;
    return true;
  }
}
