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
    std::cout << "Enter the authentication method of the server: ";
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
  bool serverAdd(Interface* inter){
    std::string name, address, port, username, authMethod, password, key;
    getServerName(name);
    getServerAddress(address);
    getServerPort(port);
    getServerUsername(username);
    getServerAuthMethod(authMethod);
    if (authMethod == "password") {
      getServerPassword(password);
    }
    else if (authMethod == "key") {
      getServerKey(key);
    }
    else{
      std::cout << "Invalid authentication method" << std::endl;
      return false;
    }
    inter->ctx->build_servers.push_back(BuildServer(name, address, port, username, authMethod, password, key));
    return true;
  }
  bool Interface::server(){
    
    if (this->args->count("subcommand") == 0) {
      serverHelp();
      
      return false;
    }
    if (this->args->operator[]("subcommand").as<std::string>() == "add") {
      serverAdd(this);
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
