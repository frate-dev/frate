#ifdef TEST
#include <catch2/catch_test_macros.hpp>
#include "../Command/Command.hpp"
#include <cxxopts.hpp>
#include <memory>
#include <string>


namespace TestCommands {
  
  bool testCommandInit() {
    std::shared_ptr<Command::Context> context = std::make_shared<Command::Context>();
    //system("mkdir /tmp/cmake-generator-test");
    context->project_path = std::filesystem::path("/tmp/cmake-generator-test");
    char* args[]{(char*)"cmake-generator",(char*)"init",(char*)"-y"};
    // for(auto& arg : args) {
    //   std::cout << arg << std::endl;
    // }
    cxxopts::ParseResult parseResults = Command::options.parse(3, args);
    std::cout << "Parse results: " << parseResults.count("init") << std::endl;
    Command::init(context, parseResults);

    bool cmakeExists = std::filesystem::exists(context->project_path / "CMakeLists.txt");
    if(cmakeExists) {
      system("rm -rf /tmp/cmake-generator-test");
      std::cout << "We found CMakeLists.txt" << std::endl;
      return true;
    }
    return false;
  }

  TEST_CASE("TestCommands", "[commands]"){

    REQUIRE(testCommandInit());
  }
}

#endif
