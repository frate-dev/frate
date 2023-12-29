#include <Frate/System/GitException.hpp>

namespace Frate::System {
  GitException::GitException(std::string message)
      : message(std::move(message)) {}

  const char *GitException::what() const noexcept { return message.c_str(); }
} // namespace Frate::System
