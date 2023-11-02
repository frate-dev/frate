#pragma once
#ifdef TEST
#include "../Command/Command.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cxxopts.hpp>
#include <memory>

namespace Tests{
  std::string genBase64String(int random_string_length);
}


namespace Tests::Command {
  bool testCommandInit();
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
  namespace TomlConfig{
  }
}
namespace Tests::Utils {


}
#endif

