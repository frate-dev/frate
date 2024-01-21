#pragma once
#include <Frate/FrateException.hpp>
namespace Frate::Utils::CLI {
  class InputTooLong : public FrateException {
  public:
    InputTooLong(const std::string &message) : FrateException(message) {}
  };

  class InputTooShort : public FrateException {
  public:
    InputTooShort(const std::string &message) : FrateException(message) {}
  };

  class InputInvalid : public FrateException {
  public:
    InputInvalid(const std::string &message) : FrateException(message) {}
  };

  class InputNotInOptions : public FrateException {
  public:
    InputNotInOptions(const std::string &message) : FrateException(message) {}
  };

  class InnputInvalidType : public FrateException {
  public:
    InnputInvalidType(const std::string &message) : FrateException(message) {}
  };
}
