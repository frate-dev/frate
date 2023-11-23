#include "Frate/Utils/Validation.hpp"
#ifdef TEST
#include <Frate/Test/Test.hpp>
#include <Frate/Generators.hpp>
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <Frate/Constants.hpp>

namespace Tests{
  std::string genBase64String(int random_string_length);
}
namespace Test::Generators {

  const std::string long_string = Tests::genBase64String(1000);

  namespace CMakeLists {
  using std::endl;
  using std::string;
  using std::cout;

  bool testCmakeVersion() {
    std::vector<std::string> failing_versions{"5.69", "5.42", "6.69", "7.77",
                                              "9.69"};
    for (std::string version : Constants::SUPPORTED_CMAKE_VERSIONS) {
      cout << "Testing version " << version << endl;
      if (!Utils::Validation::CmakeVersion(version)) {
        return false;
      }
    }
    for (std::string version : failing_versions) {
      cout << "Testing failing version " << version << endl;
      if (Utils::Validation::CmakeVersion(version)) {
        return false;
      }
    }
    return true;
    }
    bool testProjectName(){
      std::vector<std::string> mock_names{
        "Test",
        "Test-Project",
        "Test_Project",
        "TestProject",
        "TestProject123",
        "TestProject-123",
        "TestProject_123",
        "TestProject_123-123",
      };
      std::vector<std::string> failing_names{
        "Test Project",
        "Test Project^123",
        "Test Project%123",
        "Test Project$123",
        long_string,
        "Test Project_123-123",
        "Test Project&123",
        "Test Project#123",
        "Test Project@123-123",
      };

      for(std::string name : mock_names){
        cout << "Testing name " << name << endl;
        if(!Utils::Validation::ProjectName(name)){
          return false;
        }
      }

      for(std::string name : failing_names){
        cout << "Testing failing name " << name << endl;
        if(Utils::Validation::ProjectName(name)){
          return false;
        }
      }
      return true;
    };
    bool testProjectVersion(){
      std::vector<std::string> passing_project_versions = {"5.4.5","0.0.1","6.42.0","1.0.2","1.1.1", "1.1", "0.1", "0.0.1", "0.0.0", "1.0.0"};
      std::vector<std::string> failing_project_versions = {"q4.26.*","123902", "5.xx42069","420x69","42x69x420"};

      for(std::string version : passing_project_versions){
        cout << "Testing version " << version << endl;
        if(!Utils::Validation::ProjectVersion(version)){
          return false;
        }
      }
      for(std::string version : failing_project_versions){
        cout << "Testing failing version " << version << endl;
        if(Utils::Validation::ProjectVersion(version)){
          return false;
        }
      }

      return true;
    }
    bool testCLanguageVersion(){

      std::vector<std::string> failing_c_language_versions = {"3", "69", "420", "69.420", "69.420.69", "69.420.69.420"};

      for(std::string version : Constants::SUPPORTED_C_STANDARDS){
        cout << "Testing version " << version << endl;
        if(!Utils::Validation::CLanguageVersion(version)){
          return false;
        }
      }
      for(std::string version : failing_c_language_versions){
        cout << "Testing failing version " << version << endl;
        if(Utils::Validation::CLanguageVersion(version)){
          return false;
        }
      }
      return true;
    }
    bool testCppLanguageVersion(){
      std::vector<std::string> failing_cpp_language_versions = {"3", "69", "420", "69.420", "69.420.69", "69.420.69.420"};

      for(std::string version : Constants::SUPPORTED_CXX_STANDARDS){
        cout << "Testing version " << version << endl;
        if(!Utils::Validation::CppLanguageVersion(version)){
          return false;
        }
      }
      for(std::string version : failing_cpp_language_versions){
        cout << "Testing failing version " << version << endl;
        if(Utils::Validation::CppLanguageVersion(version)){
          return false;
        }
      }
      return true;
    }
    bool testCCompiler(){
      std::vector<std::string> failing_c_compilers = {"fuckcusecpp","clangdeeznuts"};

      for(std::string compiler : Constants::SUPPORTED_C_COMPILERS){
        cout << "testing c compiler: " << compiler << endl;
        if(!Utils::Validation::CCompiler(compiler)){
            return false;
        }
      }
      for(std::string compiler : failing_c_compilers){
        cout << "testing cpp failing compiler: " << compiler << endl;
        if(Utils::Validation::CCompiler(compiler)){
          return false;

        }
      }
      return true;
    }

    bool testCppCompiler(){

      std::vector<std::string> failing_cpp_compilers = {"fuckitidontknow","iforgot","icc","tcc","emcc","clang"};

      for(std::string compiler : Constants::SUPPORTED_CXX_COMPILERS){
        cout << "testing cpp compiler " << compiler << endl;
        if(!Utils::Validation::CppCompiler(compiler)){
          return false;
        }
      }
      for(std::string compiler : failing_cpp_compilers){
        cout << "testing failing cpp compiler " << compiler << endl;
        if(Utils::Validation::CppCompiler(compiler)){
          return false;
        }
      }

      return true;
    };

    bool testSourceDir(){
      std::vector<std::string> passing_source_dirs = {"src","source","sources","srcs","src_dir","source_dir","sources_dir","srcs_dir"};
      std::vector<std::string> failing_source_dirs = {"S*($#*(@","*(\&\$\^","srcs_dir//*$", long_string,"src_dir//348934","source_dir///84$#*","sources_dir*$(@#"};

      for(std::string source_dir : passing_source_dirs){
        cout << "testing passing source dir " << source_dir << endl;
        if(!Utils::Validation::SourceDir(source_dir)){
          return false;
        }
      }
      for(std::string source_dir : failing_source_dirs){
        cout << "testing failing source dir " << source_dir << endl;
        if(Utils::Validation::SourceDir(source_dir)){
          return false;
        }
      }
      return true;
    };
    bool testBuildDir(){
      std::vector<std::string> passing_build_dirs = {"build","build_dir","builds","builds_dir"};
      std::vector<std::string> failing_build_dirs = {"B*($#*(@","*(\&\$\^","builds_dir//*$","build_dir//348934", long_string,"builds_dir///84$#*","builds_dir*$(@#"};

      for(std::string build_dir : passing_build_dirs){
        cout << "testing passing build dir " << build_dir << endl;
        if(!Utils::Validation::BuildDir(build_dir)){
          return false;
        }
      }
      for(std::string build_dir : failing_build_dirs){
        cout << "testing failing build dir " << build_dir << endl;
        if(Utils::Validation::BuildDir(build_dir)){
          return false;
        }
      }
      return true;
    }
    bool testIncludeDir(){
      std::vector<std::string> passing_include_dirs = {"include","include_dir","includes","includes_dir"};
      std::vector<std::string> failing_include_dirs = {"I*($#*(@","*(\&\$\^","includes_dir//*$", long_string,"include_dir//348934","includes_dir///84$#*"};

      for(std::string include_dir : passing_include_dirs){
        if(!Utils::Validation::IncludeDir(include_dir)){
          return false;
        }
      }
      for(std::string include_dir : failing_include_dirs){
        if(Utils::Validation::IncludeDir(include_dir)){
          return false;
        }
      }
      return true;
    }
    TEST_CASE("TestGenerators", "[generators]"){
      REQUIRE(CMakeLists::testCmakeVersion());
      REQUIRE(CMakeLists::testProjectName());
      REQUIRE(CMakeLists::testProjectVersion());
      REQUIRE(CMakeLists::testCLanguageVersion());
      REQUIRE(CMakeLists::testCppLanguageVersion());
      REQUIRE(CMakeLists::testCCompiler());
      REQUIRE(CMakeLists::testCppCompiler());
      REQUIRE(CMakeLists::testSourceDir());
      REQUIRE(CMakeLists::testBuildDir());
      REQUIRE(CMakeLists::testIncludeDir());
    };
    }; // namespace CMakeLists
  namespace JsonConfig{

  };
}
#endif
