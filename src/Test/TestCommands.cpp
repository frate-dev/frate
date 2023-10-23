#ifdef TEST
#include <catch2/catch_test_macros.hpp>
#include "../Command/Command.hpp"
#include <cxxopts.hpp>
#include <memory>


namespace TestCommands {
  
 
  bool testCommandInit() {
    std::shared_ptr<Command::Context> context = std::make_shared<Command::Context>();
//    cxxopts::ParseResult args = cxxopts::ParseResult();
//    Command::init(context, args);

    return true;
  }

  TEST_CASE("TestCommands", "[commands]"){

    REQUIRE(testCommandInit());
  }
}

#endif
