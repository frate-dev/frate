#include <Frate/Command.hpp>


namespace Frate::Command::Flags {
  std::vector<std::string> makeFlags(std::shared_ptr<Interface> inter);
  bool options(std::shared_ptr<Interface> inter);
  bool add(std::shared_ptr<Interface> inter);
  bool remove(std::shared_ptr<Interface> inter);
  bool list(std::shared_ptr<Interface> inter);
}
