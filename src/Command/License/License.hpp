#pragma once
#include "../Command.hpp"


namespace Command::License {
  /*
   * set the current projet's license
   */
  bool set(Interface*, std::string& license_name);
  /*
   * set the current project's license from a file
   */
  bool set(Interface*, std::string& license_name, std::filesystem::path license_file);
  /*
   * remove the current project's license
   */
  bool remove(Interface*);
  /*
   * list all available licenses
   */
  bool list(Interface*);
  //Do this shitl
//   Command::Info getInfo(){
// 
// 
//   }
}
