#pragma once
#include "../Command.hpp"


namespace Command::RemoteServers{
  bool options(Interface* inter);
  bool add(Interface* inter);
  bool remove(Interface* inter);
  bool list(Interface* inter);
  bool get(Interface* inter);
  bool set(Interface* inter);

  //Internal functions
  std::vector<RemoteServer> remoteServerData(Interface* inter);
  bool getServerName(std::string& name);
  bool getServerUsername(std::string& username);
  bool getServerAuthMethod(std::string& authMethod);
  bool getServerAddress(std::string& address);
  bool getServerPort(int& port);
  bool getServerPassword(std::string& password);
  bool getServerKey(std::string& key);
}
