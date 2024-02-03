// LUCAS PAY ATTENTION TO IF THE HEADER IS .hpp OR .h
#ifdef TEST
#include <Frate/Test/Test.hpp>
#include <catch2/catch_session.hpp>
#else
#include <Frate/Interface.hpp>
#endif

int main(int argc, char **argv) {
  // std::shared_ptr<Command::Context> ctx =
  // std::make_shared<Command::Context>();
  

#ifdef TEST
  Catch::Session session;
  int returnCode = session.applyCommandLine(argc, argv);
  if (returnCode != 0) // Indicates a command line error
    return returnCode;
  int numFailed = session.run();
  if (numFailed == 0) {
    for (auto &p : std::filesystem::directory_iterator("/tmp")) {
      if (p.path().string().find("frate-test-") != std::string::npos) {
        std::filesystem::remove_all(p.path());
      }
    }
  }
  return (numFailed < 0xff ? numFailed : 0xff);
#else

  // TODO: Get some path checking in this bitch
  using namespace Frate::Command;


  Interface inter_ptr(argc, argv);
  auto inter = std::make_shared<Interface>(inter_ptr);
  if (execute(inter)) {
    return 0;
  }

  return -1;

#endif
}
