#ifdef TEST
#include <cstdio>
#include <filesystem>
#include <catch2/catch_test_macros.hpp>
#include <Frate/Command.hpp>
#include <cxxopts.hpp>
#include <string>
#include <Frate/Test/Test.hpp>
#include <Frate/Utils/General.hpp>
#include <Frate/Command.hpp>

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
  using nlohmann::json;

  extern std::filesystem::path test_path = std::filesystem::path("/tmp/frate-test");
  

  TEST_CASE("TestCommands", "[commands]"){
    REQUIRE(testNew());
    REQUIRE(testNewWithLang("cpp"));
    REQUIRE(testNewWithLang("c"));

    REQUIRE(testAddFlagsSingle());
    REQUIRE(testAddFlagsMultiple());
    REQUIRE(testAddFlagsWithMode());

    REQUIRE(testNewWithType("static_library"));
    REQUIRE(testNewWithType("executable"));
    REQUIRE(testNewWithType("shared_library"));
    REQUIRE(testNewWithType("header_only"));

    REQUIRE(testAddPackage("cxxopts"));
    REQUIRE(testAddPackageMultiple());
    REQUIRE(testRemovePackage("cxxopts"));
    REQUIRE(testAddPackageToMode("Debug","cxxopts"));
    REQUIRE(testAddPackageToMode("Test","cxxopts"));
    REQUIRE(testAddPackageToMode("Release","cxxopts"));

    REQUIRE(testAddAuthorSingle("test"));
    REQUIRE(testAddAuthorMultiple({"test1","test2","test3"}));
    REQUIRE(testAddDuplicateAuthor("test"));
    REQUIRE(testRemoveAuthor("test"));

    REQUIRE(testNuke());
    //REQUIRE(testCommandAdd());
  }
}

#endif
