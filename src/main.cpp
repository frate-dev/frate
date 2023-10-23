#include "Command/Command.h"
#include <iostream>
#include <memory>
#include <string>
#include <cxxopts.hpp>

/// @brief command options from user input. _must_ match map
enum cmd_options
{
  help = -1,
  i, init = 0, 
  r = 1, 
  run = 1, 
  flags = 3,
};

int main(int argc, char **argv)
{
  /*
  cxxopts::Options options("Cmake-Generator", "Stop writing CMakeLists.txt files! let us suffer for you");
  options.parse_positional({ "command", "subcommand" });
  options.add_options()
  ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"))
  ;
  auto result = options.parse(argc, argv);
  if (result.count("debug"))
  {
    std::cout << "debug enabled\n";
  }
  if (result.count("command"))
  {
    std::cout << result["command"].as<std::string>() << "\n";
  }
  */
  if (argc < 2)
  {
    Command::help();
    std::cout << "needs more args"
              << "\n";
    return 1;
  }
  #ifdef DEBUG
  std::cout << "DEBUG MODE ENABLED\n";
  #endif

  std::string command = argv[1];
  std::vector<std::string> args;
  for (int i = 2; i < argc; i++)
  {
    args.push_back(argv[i]);
  }

  // LUCAS MAKE SURE YOU INITIALIZE YOUR FUCKING STRUCT YOU TWAT
  std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();

  /// table reflects what enum cmd_options contains to switch on user input.
  static std::map<std::string, cmd_options> const table = {
      {"i", cmd_options::i},
      {"init", cmd_options::init},
      {"r", cmd_options::r},
      {"run", cmd_options::run},
      {"flags", cmd_options::flags},
      {"help", cmd_options::help},
  };
  // default if they accidentily pass nothing
  auto selection = cmd_options::help;
  auto it = table.find(command);
  if (it != table.end())
  {
    selection = it->second;
  }


  switch (selection)
  {
  case -1: // help
  default:
    Command::help();
    break;

  case 0: // i / init
    Command::init(ctx, args);
    break;

  case 1: // r / run
    Command::loadPackageToml(ctx);
    Command::run(ctx);
    break;

  case 2: // flags
    Command::loadPackageToml(ctx);
    Command::addFlag(ctx, argv[2]);
    break;
  }
  return 0;
}
