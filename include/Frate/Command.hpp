#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <sol/sol.hpp>
#include <iostream>
#include <cxxopts.hpp>
#include <Frate/Utils/CLI.hpp>
#include <Frate/Utils/Logging.hpp>
#include <Frate/Frate.hpp>
#include <Frate/Package.hpp>
#include "Frate/Utils/Config.hpp"

#define ENDL "\n"

namespace Frate::Command {

  using nlohmann::json;
  using Utils::CLI::Prompt;
  using namespace Utils::CLI::Ansi;
  using namespace cxxopts;



  namespace ProjectType {
    const std::string EXECUTABLE = "executable";
    const std::string HEADER_ONLY = "header_only";
    const std::string STATIC_LIBRARY = "static_library";
    const std::string SHARED_LIBRARY = "shared_library";
    constexpr bool validate(std::string type) {
      return type == EXECUTABLE 
        || type == HEADER_ONLY 
        || type == STATIC_LIBRARY 
        || type == SHARED_LIBRARY;
    }
  };


  //TODO: MAKE MOST OF THESE OPTIONAL


  typedef struct Handler_s Handler;
  class Interface;

  typedef struct Handler_s {
    std::vector<std::string> aliases;
    std::vector<std::string> flags{};
    std::vector<Handler> subcommands{};
    std::vector<std::string> positional_args{};
    std::string docs{""};
    std::function<bool(std::shared_ptr<Interface> )> callback{
      [](std::shared_ptr<Interface> inter) -> bool {
        (void)inter;
        std::cout << "This command has not been implemented yet" << std::endl;
        return false;
      }
    };
    bool implemented{true};
    bool requires_project{true};
    bool unlimited_args{false};
  } Handler;
  class Project;
  class Interface : public std::enable_shared_from_this<Interface>{
    public:
      Interface(int argc, char **argv);
      std::shared_ptr<Project> pro;
      bool project_present{false};
      std::shared_ptr<cxxopts::Options> options;
      std::shared_ptr<cxxopts::ParseResult> args;
      std::vector<Handler> commands{};
      Config::ConfigManager config;
      char** argv;
      int argc;
      bool confirm_all{false};
      bool parse();
      void getHelpString(std::string name,std::vector<Handler> &handlers,bool is_subcommand = false);
      void getHelpString(Handler &handler);
      bool InitHeader();
      bool CreateCMakelists();
      ~Interface();
  };

  bool execute(std::shared_ptr<Interface> inter);


  namespace OptionsInit{
      bool Main(std::shared_ptr<Interface> inter);
  };

  std::string downloadIndex();

  json fetchIndex();

  bool updateIndex();

  bool runCommand(std::shared_ptr<Interface> inter,std::string command, std::vector<Handler> &handlers);
}
