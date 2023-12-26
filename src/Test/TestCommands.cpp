#ifdef TEST
#include <catch2/catch_test_macros.hpp>
#include <cstdio>
#include <filesystem>
// #include <Frate/Interface.hpp>
#include <Frate/Test/Test.hpp>
#include <Frate/Utils/General.hpp>
#include <cxxopts.hpp>
#include <string>

namespace Tests {
  std::string genBase64String(int random_string_length) {
    std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                               "abcdefghijklmnopqrstuvwxyz"
                               "0123456789-_";

    std::string random_string;
    for (int i = 0; i < random_string_length; ++i) {
      random_string +=
          base64_chars[(rand() + time(NULL)) % base64_chars.length()];
    }
    return random_string;
  }
} // namespace Tests

namespace Tests::Command {
  using nlohmann::json;

  extern std::filesystem::path test_path =
      std::filesystem::path("/tmp/frate-test");

  TEST_CASE("TestCommands", "[commands]") {
    REQUIRE(testNew());
    REQUIRE(testNewWithLang("cpp"));
    REQUIRE(testNewWithLang("c"));

    REQUIRE(testNewWithType("executable"));
    // TODO:  Add more types
    //  REQUIRE(testNewWithType("static_library"));
    //  REQUIRE(testNewWithType("shared_library"));
    //  REQUIRE(testNewWithType("header_only"));

    REQUIRE(testAddFlagsSingle());
    REQUIRE(testAddFlagsMultiple());
    REQUIRE(testAddFlagsWithMode());
    REQUIRE(testRemoveFlagsSingle("-Wall"));
    REQUIRE(testRemoveFlagsSingle("-Wall -Werror"));

    REQUIRE(testAddPackage("cxxopts"));
    REQUIRE(testAddPackageMultiple({"cxxopts", "fmt", "SDL2"}));
    REQUIRE(testAddPackageWithDuplicate("cxxopts"));

    REQUIRE(testAddPackageToMode("Debug", "cxxopts"));
    REQUIRE(testAddPackageToMode("Test", "cxxopts"));
    REQUIRE(testAddPackageToMode("Release", "cxxopts"));

    REQUIRE(testRemoveMode("Debug"));

    REQUIRE(testRemovePackage("cxxopts"));
    REQUIRE(testRemovePackageMultiple({"fmt", "cxxopts", "SDL2"}));

    REQUIRE(testAddAuthorSingle("test"));
    REQUIRE(testAddAuthorMultiple({"test1", "test2", "test3"}));
    REQUIRE(testAddDuplicateAuthor("test"));
    REQUIRE(testRemoveAuthor("test"));

    REQUIRE(testNuke());
    // REQUIRE(testCommandAdd());
  }
} // namespace Tests::Command

#endif
