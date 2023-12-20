#pragma once
#include "../Command.hpp"
#include <Frate/RemoteServer.hpp>


namespace Frate::Command::RemoteServers{
  bool options(std::shared_ptr<Interface> inter);
  bool add(std::shared_ptr<Interface> inter);
  bool remove(std::shared_ptr<Interface> inter);
  bool list(std::shared_ptr<Interface> inter);
  bool get(std::shared_ptr<Interface> inter);
  bool set(std::shared_ptr<Interface> inter);

  //Internal functions
  std::vector<RemoteServer> remoteServerData(std::shared_ptr<Interface> inter);
  bool getServerName(std::string& name);
  bool getServerUsername(std::string& username);
  bool getServerAuthMethod(std::string& authMethod);
  bool getServerAddress(std::string& address);
  bool getServerPort(int& port);
  bool getServerPassword(std::string& password);
  bool getServerKey(std::string& key);
}
