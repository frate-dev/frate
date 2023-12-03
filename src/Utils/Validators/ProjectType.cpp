#include "Frate/Command.hpp"
namespace Frate::Utils::Validation {
  bool ProjectType(std::string type) {
    return Command::ProjectType::validate(type);
  }
}
