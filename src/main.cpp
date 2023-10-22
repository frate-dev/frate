#include "Command/Command.h"
#include <iostream>
#include <memory>
#include <string>

enum cmd_options
{
  i = 0,
  init = 1,
  r = 2,
  run = 3,
  flags = 4,
  help = 5

};

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    Command::help();
    std::cout << "needs more args"
              << "\n";
    return 1;
  }

  std::string command = argv[1];  
  std::vector<std::string> args;
  for (int i = 2; i < argc; i++)
  {
   args.push_back(argv[i]);
  }
  
  // LUCAS MAKE SURE YOU INITIALIZE YOUR FUCKING STRUCT YOU TWAT
  std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();

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
  case 0: // i / init
  case 1: 
    Command::init(ctx, args);
    break;

  case 2: // r / run
  case 3:
    Command::loadPackageToml(ctx);
    Command::run(ctx);
    break;

  case 4: // flags
    Command::loadPackageToml(ctx);
    Command::addFlag(ctx, argv[2]);
    break;

  case 5: // help
  default:
    Command::help();
    break;
  }
  return 0;
}
