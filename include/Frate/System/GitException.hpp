#pragma once
#include <exception>
#include <string>

namespace Frate::System {
  class GitException : public std::exception {
  public:
    GitException(std::string message);
    [[nodiscard]] const char *what() const noexcept override;

  private:
    std::string message;
  };
} // namespace Frate::System
