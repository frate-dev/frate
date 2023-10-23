#include "Command/Command.hpp"
#include <vector>
#include <string>
// LUCAS PAY ATTENTION TO IF THE HEADER IS .hpp OR .h
#include <cxxopts.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <string>

#ifdef TEST
  #include <catch2/catch_session.hpp>
  #include "Test/TestCommands.hpp"
#endif
int main(int argc, char **argv) {


  Command::initOptions();
  cxxopts::ParseResult result = Command::options.parse(argc, argv);

#ifdef TEST
  Catch::Session session;
  int returnCode = session.applyCommandLine(argc, argv);
  if (returnCode != 0) // Indicates a command line error
    return returnCode;
  int numFailed = session.run();
  return (numFailed < 0xff ? numFailed : 0xff);
#endif



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
  for(size_t i = 0; i < argc ; i++){
    std::cout << argv[i] << ENDL;
  }
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
