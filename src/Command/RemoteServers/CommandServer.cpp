#include <CMaker/Command.hpp>
#include <CMaker/Utils/General.hpp>
#include <CMaker/Utils/CLI.hpp>

namespace Command::RemoteServers{
  using namespace Utils::CLI;
  std::vector<RemoteServer> remoteServerData(Interface* inter){
    std::fstream file;
    std::string build_servers_dir= std::string(std::getenv("HOME"))  + "/.config/cmaker/";
    if (!std::filesystem::exists(build_servers_dir)){
      std::filesystem::create_directory(build_servers_dir);
    }
    std::string build_servers= std::string(std::getenv("HOME"))  + "/.config/cmaker/" + "build_server.json";
    std::string current_build_server= std::string(std::getenv("HOME"))  + "/.config/cmaker/" + "current_build_server.json";
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
  bool getServerName(std::string& name){
    Prompt<std::string> *name_promp = new Prompt<std::string>("Enter the name of the server: ");
    name_promp->Run();
    name = name_promp->Get();
    return true;
  }
  bool getServerAddress(std::string& address){
    Prompt<std::string> *address_promp = new Prompt<std::string>("Enter the address of the server: ");
    address_promp->Run();
    address = address_promp->Get();
    return true;
  }

  bool getServerPort(int& port){
    Prompt<int> *port_promp = new Prompt<int>("Enter the port of the server: ");
    port_promp->Run();
    port = port_promp->Get();
    return true;
  }

  bool getServerUsername(std::string& username){
    Prompt<std::string> *username_promp = new Prompt<std::string>("Enter the username of the server: ");
    username_promp->Validator([](std::string username){
        if (username == ""){
        return false;
        }
        else if (username.length() > 256){
        return false;
        }
        return true;
        });
    username_promp->Run();
    username = username_promp->Get();
    return true;
  }

  bool getServerAuthMethod(std::string& authMethod){
    Prompt<std::string> *authMethod_promp = new Prompt<std::string>("Enter the authentication method of the server[pem/password]: ");
    authMethod_promp->Run();
    authMethod = authMethod_promp->Get();
    return true;
  }

  bool getServerPassword(std::string& password){
    Prompt<std::string> *password_promp = new Prompt<std::string>("Enter the password of the server: ");
    password_promp->Run();
    password = password_promp->Get();
    return true;
  }

  bool getServerKey(std::string& key){
    Prompt<std::string> *key_promp = new Prompt<std::string>("Enter path the ssh key for the server: ");
    key_promp->Run();
    key = key_promp->Get();
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
  bool list(Interface* inter){
    //TODO put this in  the constructor
    std::vector<RemoteServer> servers =  remoteServerData(inter);
    Utils::TableFormat table;
    table.width = 20;
    table << "Name"  << "Address" << "Port" << "Username" <<  "AuthMethod" << ENDL;
    for (auto& server: servers){
      table << server.name << server.ip << server.port << server.username << server.authMethod << ENDL;
    }

    return true;
  }

  bool set(Interface* inter){
    std::vector<RemoteServer> servers =  remoteServerData(inter);
    std::string name;
    getServerName(name);
    for (auto& server: servers){
      std::cout << "server.name:" << server.name << std::endl;
      std::cout << "name:" << name << std::endl;
      if (server.name == name){

        std::cout << "Found server" << std::endl;
        std::ofstream file;
        file.open(std::string(std::getenv("HOME"))  + "/.config/cmaker/" + "current_build_server.json");
        json current_build_server = {
          {"name", server.name},
          {"address", server.ip},
          {"port", server.port},
          {"username", server.username},
          {"authMethod", server.authMethod},
          {"password", server.password.value_or("")},
          {"key", server.key.value_or("")}
        };
        file << current_build_server;
        return true;
      }
    }
    std::cout << "Could not find server" << std::endl;
    return false;
  }

  bool add(std::vector<RemoteServer> servers){
    std::string build_servers= std::string(std::getenv("HOME"))  + "/.config/cmaker/" + "build_server.json";
    std::string name, address,  username, authMethod, password, key;
    int port;
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

    RemoteServer build_server = RemoteServer(name, address, username, authMethod, password, key,  port);
    servers.push_back(build_server);

    std::vector<json> build_server_json;
    for (RemoteServer& build_server: servers){
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

  bool  get(Interface* inter){
    Utils::TableFormat table;
    table.width = 20;
    table << "Name" << "Address" << "Port" << "Username" << "AuthMethod" << ENDL;
    table << inter->pro->build_server.name << inter->pro->build_server.ip << inter->pro->build_server.port << inter->pro->build_server.username << inter->pro->build_server.authMethod << ENDL;
    return true;
  }



}
