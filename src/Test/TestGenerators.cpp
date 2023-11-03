#ifdef TEST
#include "./Test.hpp"
#include "../Generators/Generators.hpp"
#include <catch2/catch_test_macros.hpp>
#include <memory>

namespace Tests{
  std::string genBase64String(int random_string_length);
}
namespace Test::Generators {

  const std::string long_string = Tests::genBase64String(1000); 

  namespace CMakeLists {
    bool testCmakeVersion(){
      std::vector<std::string> versions{"3.28", "3.27", "3.26", "3.25", "3.24", "3.23", "3.22", "3.21", "3.20", "3.19", "3.18", "3.17", "3.16", "3.15", "3.14", "3.13", "3.12", "3.11", "3.10", "3.9", "3.8", "3.7", "3.6", "3.5", "3.4", "3.3", "3.2", "3.1", "3.0"};
      std::vector<std::string> failing_versions{"5.69", "5.42","6.69","7.77","9.69"};


      for(std::string version : versions){
        std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
        std::shared_ptr<::Generators::ConfigJson::Config> cmake_context =
          std::make_shared<::Generators::ConfigJson::Config>();
        cmake_context->cmake_version = version;
        if(!::Generators::ConfigJson::validateCmakeVersion("Testing version " + version + "\n",ctx, cmake_context)){
          return false;
        }
      }

      for(std::string version : failing_versions){
        std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
        std::shared_ptr<::Generators::ConfigJson::Config> cmake_context =
          std::make_shared<::Generators::ConfigJson::Config>();
        cmake_context->cmake_version = version;
        if(::Generators::ConfigJson::validateCmakeVersion("Testing failing version " + version + "\n",ctx, cmake_context)){
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
        std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
        std::shared_ptr<::Generators::ConfigJson::Config> cmake_context =
          std::make_shared<::Generators::ConfigJson::Config>();
        cmake_context->project_name = name;
        if(!::Generators::ConfigJson::validateProjectName("Test " + name + " \n",ctx, cmake_context)){
          return false;
        }
      }

      for(std::string name : failing_names){
        std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
        std::shared_ptr<::Generators::ConfigJson::Config> cmake_context =
          std::make_shared<::Generators::ConfigJson::Config>();
        cmake_context->project_name = name;
        if(::Generators::ConfigJson::validateProjectName("Test failing name " + name + " \n",ctx, cmake_context)){
          return false;
        }
      }
      return true;
    };
    bool testProjectVersion(){
      std::vector<std::string> passing_project_versions = {"5.4.5","0.0.1","6.42.0","1.0.2","1.1.1", "1.1", "0.1", "0.0.1", "0.0.0", "1.0.0"};
      std::vector<std::string> failing_project_versions = {"q4.26.*","123902", "5.xx42069","420x69","42x69x420"};

      for(std::string version : passing_project_versions){
        std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
        std::shared_ptr<::Generators::ConfigJson::Config> cmake_context =
          std::make_shared<::Generators::ConfigJson::Config>();
        cmake_context->project_version = version;
        if(!::Generators::ConfigJson::validateProjectVersion("Testing version " + version + "\n",ctx, cmake_context)){
          return false;
        }
      }
      for(std::string version : failing_project_versions){
        std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
        std::shared_ptr<::Generators::ConfigJson::Config> cmake_context =
          std::make_shared<::Generators::ConfigJson::Config>();
        cmake_context->project_version = version;
        if(::Generators::ConfigJson::validateProjectVersion("Testing failing version " + version + "\n",ctx, cmake_context)){
          return false;
        }
      }

      return true;
    }
    bool testLanguageVersion(){
      std::vector<std::string> passing_cpp_language_versions = {"98","11","14","17","20"};
      std::vector<std::string> failing_cpp_language_versions = {"3", "69", "420", "69.420", "69.420.69", "69.420.69.420"};

      std::vector<std::string> passing_c_language_versions = {"89","90","99","11","17"};
      std::vector<std::string> failing_c_language_versions = {"3", "69", "420", "69.420", "69.420.69", "69.420.69.420"};

      for(std::string version : passing_cpp_language_versions){
        std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
        std::shared_ptr<::Generators::ConfigJson::Config> cmake_context =
          std::make_shared<::Generators::ConfigJson::Config>();
        cmake_context->lang_version = version;
        cmake_context->lang = "cpp";
        if(!::Generators::ConfigJson::validateLanguageVersion("Testing cpp language version " + version + "\n",ctx, cmake_context)){
          return false;
        }
      }
      for(std::string version : failing_cpp_language_versions){
        std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
        std::shared_ptr<::Generators::ConfigJson::Config> cmake_context =
          std::make_shared<::Generators::ConfigJson::Config>();
        cmake_context->lang_version = version;
        cmake_context->lang = "cpp";
        if(::Generators::ConfigJson::validateLanguageVersion("Testing failing cpp language version " + version + "\n",ctx, cmake_context)){
          return false;
        }
      }
      for(std::string version : passing_c_language_versions){
        std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
        std::shared_ptr<::Generators::ConfigJson::Config> cmake_context =
          std::make_shared<::Generators::ConfigJson::Config>();
        cmake_context->lang_version = version;
        cmake_context->lang = "c";
        if(!::Generators::ConfigJson::validateLanguageVersion("Testing c language version " + version + "\n",ctx, cmake_context)){
          return false;
        }
      }
      for(std::string version : failing_c_language_versions){
        std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
        std::shared_ptr<::Generators::ConfigJson::Config> cmake_context =
          std::make_shared<::Generators::ConfigJson::Config>();
        cmake_context->lang_version = version;
        cmake_context->lang = "c";
        if(::Generators::ConfigJson::validateLanguageVersion("Testing failing c language version " + version + "\n",ctx, cmake_context)){
          return false;
        }
      }
      return true;

    };
    bool testCompiler(){
      std::vector<std::string> passing_c_compilers = {"gcc","clang","msvc","icc","tcc","emcc"};
      std::vector<std::string> failing_c_compilers = {"fuckcusecpp","clangdeeznuts"};

      std::vector<std::string> passing_cpp_compilers = {"g++","clang++"};
      std::vector<std::string> failing_cpp_compilers = {"fuckitidontknow","iforgot","msvc","icc","tcc","emcc","clang"};

      for(std::string compiler : passing_c_compilers){
        std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
        std::shared_ptr<::Generators::ConfigJson::Config> cmake_context =
          std::make_shared<::Generators::ConfigJson::Config>();
        cmake_context->compiler = compiler;
        cmake_context->lang = "c";
        if(!::Generators::ConfigJson::validateCompiler("Testing c compiler " + compiler + "\n",ctx, cmake_context)){
          return false;
        }
      }
      for(std::string compiler : failing_c_compilers){
        std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
        std::shared_ptr<::Generators::ConfigJson::Config> cmake_context =
          std::make_shared<::Generators::ConfigJson::Config>();
        cmake_context->compiler = compiler;
        cmake_context->lang = "c";
        if(::Generators::ConfigJson::validateCompiler("Testing failing c compiler " + compiler + "\n",ctx, cmake_context)){
          return false;
        }
      }
      for(std::string compiler : passing_cpp_compilers){
        std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
        std::shared_ptr<::Generators::ConfigJson::Config> cmake_context =
          std::make_shared<::Generators::ConfigJson::Config>();
        cmake_context->compiler = compiler;
        cmake_context->lang = "cpp";
        if(!::Generators::ConfigJson::validateCompiler("Testing cpp compiler " + compiler + "\n",ctx, cmake_context)){
          return false;
        }
      }
      for(std::string compiler : failing_cpp_compilers){
        std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
        std::shared_ptr<::Generators::ConfigJson::Config> cmake_context =
          std::make_shared<::Generators::ConfigJson::Config>();
        cmake_context->compiler = compiler;
        cmake_context->lang = "cpp";
        if(::Generators::ConfigJson::validateCompiler("Testing failing cpp compiler " + compiler + "\n",ctx, cmake_context)){
          return false;
        }
      }

      return true;
    };

    bool testSourceDir(){
      std::vector<std::string> passing_source_dirs = {"src","source","sources","srcs","src_dir","source_dir","sources_dir","srcs_dir"};
      std::vector<std::string> failing_source_dirs = {"S*($#*(@","*(\&\$\^","srcs_dir//*$", long_string,"src_dir//348934","source_dir///84$#*","sources_dir*$(@#"};

      for(std::string source_dir : passing_source_dirs){
        std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
        std::shared_ptr<::Generators::ConfigJson::Config> cmake_context =
          std::make_shared<::Generators::ConfigJson::Config>();

        cmake_context->src_dir = source_dir;
        if(!::Generators::ConfigJson::validateSourceDir("Testing source dir " + source_dir + "\n",ctx, cmake_context)){
          return false;
        }
      }
      for(std::string source_dir : failing_source_dirs){
        std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
        std::shared_ptr<::Generators::ConfigJson::Config> cmake_context =
          std::make_shared<::Generators::ConfigJson::Config>();

        cmake_context->src_dir = source_dir;
        if(::Generators::ConfigJson::validateSourceDir("Testing failing source dir " + source_dir + "\n",ctx, cmake_context)){
          return false;
        }
      }
      return true;
    };
    bool testBuildDir(){
      std::vector<std::string> passing_build_dirs = {"build","build_dir","builds","builds_dir"};
      std::vector<std::string> failing_build_dirs = {"B*($#*(@","*(\&\$\^","builds_dir//*$","build_dir//348934", long_string,"builds_dir///84$#*","builds_dir*$(@#"};

      for(std::string build_dir : passing_build_dirs){
        std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
        std::shared_ptr<::Generators::ConfigJson::Config> cmake_context =
          std::make_shared<::Generators::ConfigJson::Config>();

        cmake_context->build_dir = build_dir;
        if(!::Generators::ConfigJson::validateBuildDir("Testing build dir " + build_dir + "\n",ctx, cmake_context)){
          return false;
        }
      }
      for(std::string build_dir : failing_build_dirs){
        std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
        std::shared_ptr<::Generators::ConfigJson::Config> cmake_context =
          std::make_shared<::Generators::ConfigJson::Config>();

        cmake_context->build_dir = build_dir;
        if(::Generators::ConfigJson::validateBuildDir("Testing failing build dir " + build_dir + "\n",ctx, cmake_context)){
          return false;
        }
      }
      return true;
    }
    bool testIncludeDir(){
      std::vector<std::string> passing_include_dirs = {"include","include_dir","includes","includes_dir"};
      std::vector<std::string> failing_include_dirs = {"I*($#*(@","*(\&\$\^","includes_dir//*$", long_string,"include_dir//348934","includes_dir///84$#*"};

      for(std::string include_dir : passing_include_dirs){
        std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
        std::shared_ptr<::Generators::ConfigJson::Config> cmake_context =
          std::make_shared<::Generators::ConfigJson::Config>();

        cmake_context->include_dir = include_dir;
        if(!::Generators::ConfigJson::validateIncludeDir("Testing include dir " + include_dir + "\n",ctx, cmake_context)){
          return false;
        }
      }
      for(std::string include_dir : failing_include_dirs){
        std::shared_ptr<Command::Context> ctx = std::make_shared<Command::Context>();
        std::shared_ptr<::Generators::ConfigJson::Config> cmake_context =
          std::make_shared<::Generators::ConfigJson::Config>();

        cmake_context->include_dir = include_dir;
        if(::Generators::ConfigJson::validateIncludeDir("Testing failing include dir " + include_dir + "\n",ctx, cmake_context)){
          return false;
        }
      }
      return true;
    }
    TEST_CASE("TestGenerators", "[generators]"){
      REQUIRE(CMakeLists::testCmakeVersion());
      REQUIRE(CMakeLists::testProjectName());
      REQUIRE(CMakeLists::testProjectVersion());
      REQUIRE(CMakeLists::testLanguageVersion());
      REQUIRE(CMakeLists::testCompiler());
      REQUIRE(CMakeLists::testSourceDir());
      REQUIRE(CMakeLists::testBuildDir());
      REQUIRE(CMakeLists::testIncludeDir());
    };
  };
  namespace TomlConfig {

  };
}
#endif
