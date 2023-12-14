#include <Frate/Command.hpp>

namespace Frate {
  namespace Command::Completions {
    bool ZshCompletions(std::shared_ptr<Interface> inter);
    bool BashCompletions(std::shared_ptr<Interface> inter);
  }
}
