#pragma once
#include <exception>
#include <string>
#include <Frate/Utils/CLIColors.hpp>
namespace Frate {
  class FrateException : public std::exception {
  protected:
    std::string message;

  public:
    FrateException(const std::string message)
        : message(Utils::CLI::Ansi::RED + message + Utils::CLI::Ansi::RESET) {


        }

    [[nodiscard]] const char *what() const noexcept override {
      return (message.c_str());
    };
  };
}
