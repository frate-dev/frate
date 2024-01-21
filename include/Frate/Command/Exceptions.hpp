#pragma once
#include <Frate/FrateException.hpp>

namespace Frate::Command {

  class CommandException : public FrateException {
  public:
    CommandException(const std::string &message) : FrateException(message) {}
  };

  class MissingArgumentException : public CommandException {
  public:
    MissingArgumentException(const std::string &message)
        : CommandException(message) {}
  };

  class InvalidArgumentValueException : public CommandException {
  public:
    InvalidArgumentValueException(const std::string &message)
        : CommandException(message) {}
  };

  class InvalidArgumentTypeException : public CommandException {
  public:
    InvalidArgumentTypeException(const std::string &message)
        : CommandException(message) {}
  };

  class InvalidCommandException : public CommandException {
  public:
    InvalidCommandException(const std::string &message)
        : CommandException(message) {}
  };

  class InvalidSubcommandException : public CommandException {
  public:
    InvalidSubcommandException(const std::string &message)
        : CommandException(message) {}
  };

  class ActionNotImplementedException : public CommandException {
  public:
    ActionNotImplementedException(const std::string &message)
        : CommandException(message) {}
  };

  class HandlerNotImplementedException : public CommandException {
  public:
    HandlerNotImplementedException(const std::string &message)
        : CommandException(message) {}
  };

} // namespace Frate::Command
