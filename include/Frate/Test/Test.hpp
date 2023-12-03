#include "Frate/Utils/General.hpp"
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
  extern Frate::Utils::Error error;
  extern Frate::Utils::Info info;
  extern Frate::Utils::Warning warning;
  void cleanUp(path test_path);

  std::pair<bool, Frate::Command::Interface*> init(std::string command,bool check_config=false);

  bool validateProjectJson(Frate::Command::Interface* inter);

  std::pair<int, char**> genCommand(std::string args);
  bool testCommandInit();
  bool testNew();
  bool testNewWithLang(std::string lang);
  bool testNewWithType(std::string type);

  bool testAddFlagsMultiple();
  bool testAddFlagsSingle();
  bool testAddFlagsWithMode();

  bool testAddPackage(std::string package_name);
  bool testAddPackageMultiple();
  bool testAddPackageToMode(std::string mode_name, std::string package_name);
  bool testRemovePackage(std::string package_name);

  bool testAddAuthorMultiple(std::vector<std::string> name);
  bool testAddAuthorSingle(std::string name);
  bool testAddDuplicateAuthor(std::string name);
  bool testRemoveAuthor(std::string name);

  bool testNuke();
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

