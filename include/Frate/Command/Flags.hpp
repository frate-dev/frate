#include <Frate/Command.hpp>


namespace Command::Flags {
  std::vector<std::string> makeFlags(Interface *inter);
  bool options(Interface* inter);
  bool add(Interface *inter);
  bool remove(Interface *inter);
}
