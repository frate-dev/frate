#include <CMaker/Command.hpp>
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
  #include <CMaker/Test/Test.hpp>
#endif
int main(int argc, char **argv) {


  // LUCAS MAKE SURE YOU INITIALIZE YOUR F@*KING STRUCT YOU TW@T
  //std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
  #ifdef TEST
    Catch::Session session;
    int returnCode = session.applyCommandLine(argc, argv);
    if (returnCode != 0) // Indicates a command line error
      return returnCode;
    int numFailed = session.run();
    return (numFailed < 0xff ? numFailed : 0xff);
  #else

  //TODO: Get some path checking in this bitch
  new Command::Interface(argc,argv);

  #endif
}
