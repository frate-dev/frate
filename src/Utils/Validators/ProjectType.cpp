#include "Frate/Command.hpp"
namespace Utils::Validation {
  bool ProjectType(std::string type) {
    return Command::ProjectType::validate(type);
  }
}
