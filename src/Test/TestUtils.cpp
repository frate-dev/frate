#ifdef TEST
#include <Frate/Frate.hpp>
#include <Frate/Utils/General.hpp>
#include <Frate/Utils/Logging.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>

namespace Tests::Utils {

  bool testSplit() {
    std::vector<std::string> mock_strings = {
        "fuck I don't even know",
        "shit",
        "gotta write these fucking tests",
        "this is a sentence to test the split functionality"};
    std::vector<size_t> expected_results = {5, 1, 5, 9};

    for (size_t i = 0; i < mock_strings.size(); i++) {
      if (!(Frate::Utils::split(mock_strings[i], ' ').size() ==
            expected_results[i])) {
        Frate::Utils::info << "Expected: " << expected_results[i] << "Got: "
                           << Frate::Utils::split(mock_strings[i], ' ').size()
                           << std::endl;
        return false;
      }
    }
    return true;
  }

  bool testToLower() {
    std::vector<std::string> mock_strings = {
        "Fuck", "fuCk THis Test", "This funtion causes SO MANY PROBLEMS"};
    std::vector<std::string> expected_strings = {
        "fuck", "fuck this test", "this funtion causes so many problems"};
    for (std::string &str : mock_strings) {
      Frate::Utils::toLower(str);
    }
    for (size_t i = 0; i < mock_strings.size(); i++) {
      if (mock_strings[i] != expected_strings[i]) {
        Frate::Utils::info << "Expected: " << expected_strings[i]
                           << " Got: " << mock_strings[i] << std::endl;
        return false;
      }
    }
    return true;
  }

  TEST_CASE("Utils testing", "[utils-test]") {
    REQUIRE(testSplit());
    REQUIRE(testToLower());
  }
} // namespace Tests::Utils
#endif
