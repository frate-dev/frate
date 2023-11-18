#pragma once
#include "../Command.hpp"


namespace Command{
  namespace RemoteServers{
    
    bool add(std::vector<RemoteServer> servers);
    bool list(std::vector<RemoteServer> servers);
    bool get(Interface* inter);
    bool set(std::vector<RemoteServer> servers);
  }
}
