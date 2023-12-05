#ifdef TEST
#pragma once
#include "../Command.hpp"
#include "Frate/Utils/General.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cxxopts.hpp>

namespace Tests{
  std::string genBase64String(int random_string_length);
}


namespace Tests::Command {
  using std::filesystem::path;
  extern std::filesystem::path test_path;
  extern Frate::Utils::Error error;
  extern Frate::Utils::Info info;
  extern Frate::Utils::Warning warning;
  void cleanUp(path test_path);

  std::pair<bool, Frate::Command::Interface*> init(std::string command,bool check_config=false);

  bool validateProjectJson(Frate::Command::Interface* inter);

  std::filesystem::path genTestDirectory();
  std::pair<int, char**> genCommand(std::string args);
  /*
   * ------------------
   * Test Init Commands
   * ------------------
   */
  bool testNew(); // Used to create new project in all tests integ tests
  bool testNewWithLang(std::string lang);
  bool testNewWithType(std::string type);

  /*
   * ------------------
   * Test Flag commands
   * ------------------
   */
  bool testAddFlagsMultiple();
  bool testAddFlagsSingle();
  bool testAddFlagsWithMode();

  bool testRemoveFlagsSingle(std::string flag);
  
  /* --------------------
   * Test Package Commands
   * --------------------
   */
  bool testAddPackage(std::string package_name);
  bool testAddPackageMultiple(std::vector<std::string> packages);
  bool testAddPackageWithDuplicate(std::string package_name);
  bool testAddPackageToMode(std::string mode_name, std::string package_name);

  bool testRemovePackage(std::string package_name);
  bool testRemovePackageMultiple(std::vector<std::string> package_names);

  /*
   * --------------------
   *  Test Mode Commands
   *  -------------------
   */
  bool testAddMode(std::string mode_name);
  bool testAddDuplicateMode(std::string mode_name);

  bool testRemoveMode(std::string mode_name);

  /* --------------------
   * Test Author Commands
   * --------------------
   */
  bool testAddAuthorMultiple(std::vector<std::string> name);
  bool testAddAuthorSingle(std::string name);
  bool testAddDuplicateAuthor(std::string name);
  bool testRemoveAuthor(std::string name);

  /* --------------------
   * Test Nuke Command
   * --------------------
   */
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

