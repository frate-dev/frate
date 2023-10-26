
#ifdef TEST
#include <cstdio>
#include <filesystem>
#include <catch2/catch_test_macros.hpp>
#include "../Command/Command.hpp"
#include <cxxopts.hpp>
#include <memory>
#include <string>
#include "Test.hpp"

namespace Tests{
  std::string genBase64String(int random_string_length) {
    std::string base64_chars =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz"
      "0123456789-_";

    std::string random_string;
    for (int i = 0; i < random_string_length; ++i) {
      random_string += base64_chars[(rand() + time(NULL)) % base64_chars.length()];
    }
    return random_string;
  }
}



namespace Tests::Command {


  
  bool testCommandInit() { std::shared_ptr<::Command::Context> context = std::make_shared<::Command::Context>();
    char* args[] = {"cmake-generator","init","-y"};


    for(auto arg : args) {
      std::cout << arg << std::endl;
    }
    cxxopts::Options options(
      "Cmake-Generator",
      "Stop writing CMakeLists.txt files! let us suffer for you");
    ::Command::initOptions(options);
    cxxopts::ParseResult parseResults = options.parse(3, args);
    ::Command::init(context, parseResults);
    std::ifstream file;
    try{
      file = std::ifstream(context->project_path / "CMakeLists.txt");
    } catch (std::exception& e) {
      std::cout << "Failed to load CMakeLists.txt" << std::endl;
      return false;
    }
    return true;
  }



  TEST_CASE("TestCommands", "[commands]"){

    REQUIRE(testCommandInit());
  }
}

#endif
