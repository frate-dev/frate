#include "Command/Command.hpp"
#include <vector>
#include <string>
// LUCAS PAY ATTENTION TO IF THE HEADER IS .hpp OR .h
#include <cxxopts.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <string>

#ifdef TEST
  #include <catch2/catch_session.hpp>
  #include "Test/Test.hpp"
#endif
int main(int argc, char **argv) {

  // LUCAS MAKE SURE YOU INITIALIZE YOUR FUCKING STRUCT YOU TWAT
  std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
  cxxopts::ParseResult result = Command::mainOptions(argc, argv);

  #ifdef TEST
    Catch::Session session;
    int returnCode = session.applyCommandLine(argc, argv);
    if (returnCode != 0) // Indicates a command line error
      return returnCode;
    int numFailed = session.run();
    return (numFailed < 0xff ? numFailed : 0xff);
  #else

  if (!result.count("command")) {
    Command::help();
    return 1;
  }

  std::string command = result["command"].as<std::string>() ;
 
  #ifdef DEBUG
    std::cout << "DEBUG MODE ENABLED\n";
  #endif

  using namespace cxxopts;
  if (command == "init"){
    ParseResult options = Command::initOptions(argc, argv);
    for (auto option : options.arguments())
      std::cout << option.key()  <<option.value() << ENDL;
    
    Command::init(ctx, options);
  }
  else if (command == "run"){
    Command::loadPackageToml(ctx);
    Command::run(ctx);
  }
  else if (command == "help"){
    Command::help();
  }

  else if (command == "ftp"){
    Command::ftp(ctx);
  }
  else if (command == "add"){
    cxxopts::ParseResult args = Command::addOptions(argc, argv);
    for (auto &i : args.arguments()){
      std::cout << i.key() << " " << i.value() << ENDL;
    }
    Command::loadPackageToml(ctx);
    Command::add(ctx, args);

  }
  else if (command == "remove"){
    ParseResult options = Command::removeOptions(argc, argv);
    Command::loadPackageToml(ctx);
    Command::remove(ctx, options);
  }
  else if (command == "dev"){
    Command::loadPackageToml(ctx);
    Command::dev(ctx);
  }
  else if (command == "update"){
    ParseResult options = Command::updateOptions(argc, argv);
    Command::update(ctx, result);
  }
  else{
    std::cout << "Invalid command try one of these" << ENDL;
    Command::help();

  }
#endif
}
