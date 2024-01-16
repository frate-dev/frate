#pragma once
#include <cstdint>
#include <iostream>
#include <termcolor/termcolor.hpp>

namespace Frate::Utils {
  extern bool verbose_mode;

  class Error {
  public:
    template <typename T> Error &operator<<(const T &data) {
      std::cout << termcolor::red << data << termcolor::reset;
      return *this;
    }

    Error &operator<<(std::ostream &(*pf)(std::ostream &)) {
      std::cout << pf;
      return *this;
    }
  };

  class Warning {
  public:
    template <typename T> Warning &operator<<(const T &data) {
      std::cout << termcolor::yellow << data << termcolor::reset;
      return *this;
    }

    Warning &operator<<(std::ostream &(*pf)(std::ostream &)) {
      std::cout << pf;
      return *this;
    }
  };

  class Info {
  public:
    template <typename T> Info &operator<<(const T &data) {
      std::cout << termcolor::green << data << termcolor::reset;
      return *this;
    }

    Info &operator<<(std::ostream &(*pf)(std::ostream &)) {
      std::cout << pf;
      return *this;
    }
  };

  class Verbose {
  public:
    template <typename T> Verbose &operator<<(const T &data) {
      if (verbose_mode) {
        std::cout << termcolor::bright_blue << data << termcolor::reset;
      }
      return *this;
    }

    Verbose &operator<<(std::ostream &(*pf)(std::ostream &)) {
      if (verbose_mode) {
        std::cout << pf;
      }
      return *this;
    }
  };

  extern Utils::Error error;
  extern Utils::Info info;
  extern Utils::Warning warning;
  extern Utils::Verbose verbose;
} // namespace Frate::Utils
