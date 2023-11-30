#ifdef TEST
#pragma once
#include "../Command.hpp"

#include <catch2/catch_test_macros.hpp>
#include <cxxopts.hpp>
#include <memory>

namespace Tests{
  std::string genBase64String(int random_string_length);
}


namespace Tests::Command {
  using std::filesystem::path;
  extern const std::filesystem::path test_path;
  void cleanUp(path test_path);

  void init(path test_path);

  bool validateProjectJson(::Command::Interface* inter);

  std::pair<int, char**> genCommand(std::string args);
  bool testCommandInit();
  bool testNew();
  bool testNewWithLang(std::string lang);
  bool testNewWithType(std::string type);

  bool testFlagsMultiple();
  bool testFlagsSingle();
  bool testFlagsWithMode();

  bool testAddPackage(std::string package_name);

  bool testAddPackageMultiple();

  bool testRemovePackage();
  bool testAddPackageToMode(std::string mode_name, std::string package_name);
}

namespace Tests::Generators{
  namespace CmakeLists{

    bool testCmakeVersion();
    bool testProjectName();
    bool testProjectVersion();
    bool testLanguageVersion();
    bool testCompiler();
    bool testSourceDir();
    bool testBuildDir();
    bool testIncludeDir();
  }
  namespace JsonConfig{
  }
}
namespace Tests::Utils {

  bool testSplit();

  bool testToLower();

}
#endif

