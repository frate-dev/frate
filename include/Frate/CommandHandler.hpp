#pragma once
#include "Frate/Command/Exceptions.hpp"
#include <cxxopts.hpp>
#include <memory>
#include <string>
#include <vector>


namespace Frate::Command {
  class Interface;

  class CommandHandler {
  private:
    std::vector<std::string> aliases;
    std::vector<cxxopts::Option> options;
    std::vector<std::unique_ptr<CommandHandler>> subcommands;
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

    std::vector<cxxopts::Option> &getOptions() { return options; }

    std::vector<std::unique_ptr<CommandHandler>> &getSubcommands() {
      return subcommands;
    }

    std::vector<std::string> &getPosArgs() { return positional_args; }

    std::string &getDocs() { return docs; }

    std::shared_ptr<Interface> &getInterface() { return inter; }

    // Setters

    void setAliases(std::vector<std::string> &aliases) {
      this->aliases = aliases;
    }

    void
    setSubcommands(std::vector<std::unique_ptr<CommandHandler>> subcommands) {
      this->subcommands = std::move(subcommands);
    }

    void setPosArgs(std::vector<std::string> &positional_args) {
      this->positional_args = positional_args;
    }

    void setDocs(std::string &docs) { this->docs = docs; }

    void setInterface(std::shared_ptr<Interface> &inter) {
      this->inter = inter;
    }

    // Adders

    void addSubcommand(std::unique_ptr<CommandHandler> subcommand) {
      subcommands.push_back(std::move(subcommand));
    }

    void addOption(cxxopts::Option option) { options.push_back(option); }

    void addOptions(std::initializer_list<cxxopts::Option> options) {
      for (auto &option : options) {
        this->options.push_back(option);
      }
    }
  };

} // namespace Frate::Command
