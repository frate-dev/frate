#ifdef TEST
#include <cstdio>
#include <filesystem>
#include <catch2/catch_test_macros.hpp>
#include <CMaker/Command.hpp>
#include <cxxopts.hpp>
#include <memory>
#include <string>
#include <CMaker/Test/Test.hpp>

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
  using ::Command::Project;
  using ::Command::Interface;





//  [[deprecated("This test is deprecated because it is not a unit test")]]  
//  bool testCommandInit() { std::shared_ptr<::Command::Context> context = std::make_shared<::Command::Context>();
//    char* args[] = {"cmake-generator","init","-y"};
//
//    for(auto arg : args) {
//      std::cout << arg << std::endl;
//    }
//    cxxopts::ParseResult parseResults = ::Command::initOptions(3, args);
//    ::Command::init(context, parseResults);
//    std::ifstream file;
//    try{
//      file = std::ifstream(context->project_path / "CMakeLists.txt");
//    } catch (std::exception& e) {
//      std::cout << "Failed to load CMakeLists.txt" << std::endl;
//      return false;
//    }
//    return true;
//  }
//  [[deprecated("This test is deprecated because it is not a unit test")]]
//  bool testCommandAdd() { std::shared_ptr<::Command::Context> context = std::make_shared<::Command::Context>();
//    char* args[] = {"cmaker","add","dep", "cxxopts"};
//
//    for(auto arg : args) {
//      std::cout << arg << std::endl;
//    }
//    cxxopts::ParseResult parseResults = ::Command::initOptions(3,args);
//    ::Command::init(context, parseResults);
//    std::ifstream file;
//    try{
//      file = std::ifstream(context->project_path / "CMakeLists.txt");
//    } catch (std::exception& e) {
//      std::cout << "Failed to load CMakeLists.txt" << std::endl;
//      return false;
//    }
//    
//    return true;
//  }



  TEST_CASE("TestCommands", "[commands]"){

    //REQUIRE(testCommandInit());
    //REQUIRE(testCommandAdd());
  }
}

#endif
