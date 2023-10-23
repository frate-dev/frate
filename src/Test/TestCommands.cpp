#ifdef TEST
#include <catch2/catch_test_macros.hpp>
#include "../Command/Command.hpp"
#include <cxxopts.hpp>
#include <memory>
#include <string>


namespace TestCommands {

  std::string genBase64String() {
    std::string base64_chars =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz"
      "0123456789-_";

    std::string random_string;
    int random_string_length = 10;
    for (int i = 0; i < random_string_length; ++i) {
      random_string += base64_chars[rand() % base64_chars.length()];
    }
    return random_string;
  }
  
  bool testCommandInit() {
    std::shared_ptr<Command::Context> context = std::make_shared<Command::Context>();
    context->project_path = std::filesystem::path("/tmp/cmake-generator-test" + genBase64String());
    std::string mkdir = ("mkdir " + context->project_path.string());
    system(mkdir.c_str());
    char* args[] = {"cmake-generator","init","-y"};

    for(auto arg : args) {
      std::cout << arg << std::endl;
    }
    cxxopts::Options options(
      "Cmake-Generator",
      "Stop writing CMakeLists.txt files! let us suffer for you");
    Command::initOptions(options);
    cxxopts::ParseResult parseResults = options.parse(3, args);
    Command::init(context, parseResults);

    bool cmakeExists = std::filesystem::exists(context->project_path / "config.toml");
    std::cout << context->project_path / "CMakeLists.txt and it exists: " << cmakeExists << std::endl;
    if(cmakeExists) {
      std::string rm = ("rm -rf " + context->project_path.string());
      system(rm.c_str());
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
