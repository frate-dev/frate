#pragma once
#include "../Command.hpp"


namespace Command{
  namespace RemoteServers{
    
    bool add(std::vector<RemoteServer> servers);
    bool serverList(std::vector<RemoteServer> servers);
    bool getRemoteServer(Interface* inter);
    bool setRemoteServer(std::vector<RemoteServer> servers);
  }
}
