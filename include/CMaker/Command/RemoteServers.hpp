#pragma once
#include "../Command.hpp"


namespace Command{
  namespace RemoteServers{
    
    bool add(Interface* inter);
    bool list(Interface* inter);
    bool get(Interface* inter);
    bool set(Interface* inter);
  }
}
