#include "Command.hpp"
#include "../Utils/General.hpp"
namespace Command{
  bool getServerName(std::string& name){
    std::cout << "Enter the name of the server: ";
    std::getline(std::cin, name);
    return true;
  }
  bool getServerAddress(std::string& address){
    std::cout << "Enter the address of the server: ";
    std::getline(std::cin, address);
    return true;
  }
  bool getServerPort(std::string& port){
    std::cout << "Enter the port of the server: ";
    std::getline(std::cin, port);
    return true;
  }
  bool getServerUsername(std::string& username){
    std::cout << "Enter the username of the server: ";
    std::getline(std::cin, username);
    return true;
  }
  bool getServerAuthMethod(std::string& authMethod){
    std::cout << "Enter the authentication method of the server[pem/password]: ";
    std::getline(std::cin, authMethod);
    return true;
  }
  bool getServerPassword(std::string& password){
    std::cout << "Enter the password of the server: ";
    std::getline(std::cin, password);
    return true;
  }
  bool getServerKey(std::string& key){
    std::cout << "Enter path the ssh key for the server: ";
    std::getline(std::cin, key);
    return true;
  }

  bool serverHelp(){

    std::cout << R"EOF(
Usage server:
  add: adds a  build server
  remove: removes a build server
  list: lists all build servers
  set: sets the current build server
  get: gets the current build server
        )EOF" << std::endl;
    return true;
  }
  bool serverAdd(std::vector<BuildServer> servers){

    std::string build_servers= std::string(std::getenv("HOME"))  + "/.config/cmaker/" + "build_server.json";
    std::string name, address, port, username, authMethod, password, key;
    getServerName(name);
    getServerAddress(address);
    getServerPort(port);
    getServerUsername(username);
    getServerAuthMethod(authMethod);
    if (authMethod == "password") {
      getServerPassword(password);
    }
    else if (authMethod == "pem") {
      getServerKey(key);
    }
    else{
      std::cout << "Invalid authentication method" << std::endl;
      return false;
    } 



   BuildServer build_server = BuildServer(name, address, username, authMethod, password, key,  std::stoi(port));
   servers.push_back(build_server);

  std::vector<json> build_server_json;
   for (auto& build_server: servers){
     json build_server_json_tmp = {
       {"name", build_server.name},
       {"address", build_server.ip},
       {"port", build_server.port},
       {"username", build_server.username},
       {"authMethod", build_server.authMethod},
       {"password", build_server.password.value_or("")},
       {"key", build_server.key.value_or("")}
     }; 
     build_server_json.push_back(build_server_json_tmp);
   }
    std::ofstream file;
    file.open(build_servers);
    file << build_server_json;
    return true;
  }
  bool Interface::server(){
    std::fstream file;
    std::string build_servers_dir= std::string(std::getenv("HOME"))  + "/.config/cmaker/";
    if (!std::filesystem::exists(build_servers_dir)){
      std::filesystem::create_directory(build_servers_dir);
    }
    std::string build_servers= std::string(std::getenv("HOME"))  + "/.config/cmaker/" + "build_server.json";
    if (!std::filesystem::exists(build_servers)){
      std::ofstream file(build_servers);
      file << "[]";
    }
    file.open(build_servers);

    std::vector<BuildServer> servers;
    json server_list;
    try{
      server_list = json::parse(file);
    }
    catch(json::exception &e){
      std::cout << "Error: Could not load build_server.json" << std::endl;
      return false;
    }



    for (auto& server : server_list){
        BuildServer  build_server = BuildServer(
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

    
    if (this->args->count("subcommand") == 0) {
      serverHelp();
      
      return false;
    }
    if (this->args->operator[]("subcommand").as<std::string>() == "add") {
      serverAdd(servers);
    }
    else if (this->args->operator[]("subcommand").as<std::string>() == "remove") {
    }
    else if (this->args->operator[]("subcommand").as<std::string>() == "list") {
    }
    else if (this->args->operator[]("subcommand").as<std::string>() == "set") {
    }
    else if (this->args->operator[]("subcommand").as<std::string>() == "get") {
    }
    else{
      serverHelp();

      return false;
    }
    return true;
  }
}
