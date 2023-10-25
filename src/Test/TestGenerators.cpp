#ifdef TEST
#include "../Generators/Generators.hpp"
#include <catch2/catch_test_macros.hpp>
#include <memory>

namespace Test::Generators {
  namespace CMakeLists {
    bool testCmakeVersion(){
      std::vector<std::string> versions{"3.28", "3.27", "3.26", "3.25", "3.24", "3.23", "3.22", "3.21", "3.20", "3.19", "3.18", "3.17", "3.16", "3.15", "3.14", "3.13", "3.12", "3.11", "3.10", "3.9", "3.8", "3.7", "3.6", "3.5", "3.4", "3.3", "3.2", "3.1", "3.0"};
      for(std::string version : versions){
        std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
        std::shared_ptr<::Generators::ConfigToml::ConfigToml> cmake_context =
          std::make_shared<::Generators::ConfigToml::ConfigToml>();
        cmake_context->cmake_version = version;
        if(::Generators::ConfigToml::cmakeVersion("Test",ctx, cmake_context)){
          return true;
        }
      }
      return false;
    }
    bool testProjectName(){

    };
    bool testProjectVersion();
    bool testLanguageVersion();
    bool testCompiler();
    bool testSourceDir();
    bool testBuildDir();
    bool testIncludeDir();
    TEST_CASE("TestGenerators", "[generators]"){
      REQUIRE(CMakeLists::testCmakeVersion());
      // REQUIRE(CMakeLists::testProjectName());
      // REQUIRE(CMakeLists::testProjectVersion());
      // REQUIRE(CMakeLists::testLanguageVersion());
      // REQUIRE(CMakeLists::testCompiler());
      // REQUIRE(CMakeLists::testSourceDir());
      // REQUIRE(CMakeLists::testBuildDir());
      // REQUIRE(CMakeLists::testIncludeDir());
    };
  };
  namespace TomlConfig {

  };
}
#endif
