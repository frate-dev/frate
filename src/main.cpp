#include "Frate/Utils/General.hpp"
#include <Frate/Command.hpp>
#include <Frate/Frate.hpp>
// LUCAS PAY ATTENTION TO IF THE HEADER IS .hpp OR .h
#ifdef TEST
  #include <catch2/catch_session.hpp>
  #include <Frate/Test/Test.hpp>
#endif


namespace Frate {
  Utils::Error error = Utils::Error();
  Utils::Info info = Utils::Info();
  Utils::Warning warning = Utils::Warning();
}

int main(int argc, char **argv) {
  // LUCAS MAKE SURE YOU INITIALIZE YOUR F@*KING STRUCT YOU TW@T
  //std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
  
  #ifdef TEST
    Catch::Session session;
    int returnCode = session.applyCommandLine(argc, argv);
    if (returnCode != 0) // Indicates a command line error
      return returnCode;
    int numFailed = session.run();
    if(numFailed == 0){
      for(auto &p : std::filesystem::directory_iterator("/tmp")){
        if(p.path().string().find("frate-test-") != std::string::npos){
          std::filesystem::remove_all(p.path());
        }
      }
    }
    return (numFailed < 0xff ? numFailed : 0xff);
  #else

    //TODO: Get some path checking in this bitch
    using namespace Frate::Command;

    Interface *inter_ptr = new Interface(argc,argv);
    auto inter = std::make_shared<Interface>(*inter_ptr);
    if(execute(inter)){
      return 0;
    }else{
      return -1;
    }


  #endif
}
