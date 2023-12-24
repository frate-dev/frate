#include "Frate/Utils/General.hpp"
#include <Frate/Command/RemoteServers.hpp>
#include <Frate/Interface.hpp>

namespace Frate::Command::RemoteServers{
  bool list(std::shared_ptr<Interface> inter){
    //TODO put this in  the constructor
    // std::vector<RemoteServer> servers =  remoteServerData(inter);
    Utils::TableFormat table;
    table.width = 20;
    table << "Name"  << "Address" << "Port" << "Username" <<  "AuthMethod" << "\n";
    for (auto& server: inter->config.build_servers){
      table << server.name << server.ip << server.port << server.username << server.authMethod << "\n";
    }

    return true;
  }
}
