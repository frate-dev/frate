#pragma once
#include "Frate/Utils/Logging.hpp"
#include <Frate/Project/Config.hpp>
#include <Frate/Utils/Config.hpp>
#include <cxxopts.hpp>
#include <memory>
#include <string>
#include <vector>

namespace Frate::Command {
  class Interface;
  using Handler = struct Handler_s;
  /* =============================
   * Future interface for commands
   * =============================
   */
  class CommandHandler;

  class CommandHandler {
  public:
    virtual ~CommandHandler() = default;
    std::vector<std::string> aliases;
    std::vector<std::string> flags{};
    std::vector<CommandHandler> subcommands{};
    std::vector<std::string> positional_args{};
    bool implemented{true};
    bool requires_project{true};
    bool unlimited_args{false};
    std::string docs;
    std::function<bool(std::shared_ptr<Interface>)> callback{
        [](std::shared_ptr<Interface> inter) -> bool {
          (void)inter;
          Utils::error << "This command has not been implemented yet"
                       << std::endl;
          return false;
        }};
    virtual bool run(std::shared_ptr<Interface> inter) = 0;
    virtual bool options(std::shared_ptr<Interface> inter) = 0;
  };

  using Handler = struct Handler_s {
    std::vector<std::string> aliases;
    std::vector<std::string> flags{};
    std::vector<Handler> subcommands{};
    std::vector<std::string> positional_args{};
    std::string docs;
    std::function<bool(std::shared_ptr<Interface>)> callback{
        [](std::shared_ptr<Interface> inter) -> bool {
          (void)inter;
          Utils::error << "This command has not been implemented yet"
                       << std::endl;
          return false;
        }};
    bool implemented{true};
    bool requires_project{true};
    bool unlimited_args{false};
  };

  class Interface : public std::enable_shared_from_this<Interface> {
  public:
    Interface(int argc, char **argv);
    std::shared_ptr<Project::Config> pro;
    bool project_present{false};
    std::shared_ptr<cxxopts::Options> options;
    std::shared_ptr<cxxopts::ParseResult> args;
    std::vector<Handler> commands{};
    std::shared_ptr<Config::ConfigManager> config;
    std::shared_ptr<Project::TemplateManager> templates;
    char **argv;
    int argc;
    bool confirm_all{false};
    bool parse();
    void getHelpString(std::string name, std::vector<Handler> &handlers,
                       bool is_subcommand = false);
    void getHelpString(Handler &handler);
    bool InitHeader();
    bool CreateCMakelists();
    ~Interface();
  };

  bool execute(std::shared_ptr<Interface> inter);

  namespace OptionsInit {
    bool Main(std::shared_ptr<Interface> inter);
  };

  bool runCommand(std::shared_ptr<Interface> inter, std::string command,
                  std::vector<Handler> &handlers);
} // namespace Frate::Command
