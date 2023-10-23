#include "Command/Command.h"
#include <vector>
#include <string>
#include <cxxopts.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <string>

int main(int argc, char **argv) {

  cxxopts::Options options(
      "Cmake-Generator",
      "Stop writing CMakeLists.txt files! let us suffer for you");

  options.parse_positional({"command",  "subcommand"});
  options.add_options()
    ("p,production", "Enable debug mode", cxxopts::value<bool>()->default_value("false"))
    ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
    ("subcommand", "Subcommand to run", cxxopts::value< std::vector<std::string>>())
    ("h,help", "Print usage")
    ("y,skip-init", "skip init", cxxopts::value<bool>()->default_value("false"))
    ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"));

  auto result = options.parse(argc, argv);


  if (!result.count("command")) {
    Command::help();
    return 1;
  }

  std::string command = result["command"].as<std::string>() ;
  if (argc < 2) {
    Command::help();
    std::cout << "needs more args"
              << "\n";
    return 1;
  }
  #ifdef DEBUG
    std::cout << "DEBUG MODE ENABLED\n";
  #endif

  // LUCAS MAKE SURE YOU INITIALIZE YOUR FUCKING STRUCT YOU TWAT
  std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
  if (command == "init"){
    Command::init(ctx, result);
  }
  else if (command == "run"){
    Command::loadPackageToml(ctx);
    Command::run(ctx);
  }
  else if (command == "help"){
    Command::help();
  }
  else if (command == "flags"){
    Command::loadPackageToml(ctx);
    Command::addFlag(ctx, result);

  }
  else if (command == "ftp"){
    Command::ftp(ctx);
  }
}
