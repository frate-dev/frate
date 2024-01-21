#pragma once
#include "Frate/Command/Exceptions.hpp"
#include <memory>
#include <string>
#include <vector>

namespace Frate::Command {
  class Interface;

  class CommandHandler {
  protected:
    std::vector<std::string> aliases;
    std::vector<std::string> flags{};
    std::vector<CommandHandler> subcommands{};
    std::vector<std::string> positional_args{};
    bool implemented{true};
    bool requires_project{true};
    bool unlimited_args{false};
    std::string docs;
    std::shared_ptr<Interface> inter;

  public:
    CommandHandler(std::shared_ptr<Interface> inter) : inter(inter) {}

    virtual ~CommandHandler() = default;

    virtual void run() = 0;
    virtual void registerOptions() = 0;
    virtual void checkInput() = 0;

    // Checkers
    bool isImplemented() { return implemented; }
    bool requiresProject() { return requires_project; }
    bool hasUnlimitedArgs() { return unlimited_args; }

    // Getters
    std::vector<std::string> &getAliases() { return aliases; }
    std::vector<std::string> &getFlags() { return flags; }
    std::vector<CommandHandler> &getSubcommands() {
      return subcommands;
    }

    std::vector<std::string> &getPosArgs() { return positional_args; }
    std::string &getDocs() { return docs; }
    std::shared_ptr<Interface> &getInterface() { return inter; }
  };

} // namespace Frate::Command
