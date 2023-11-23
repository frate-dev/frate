#include "Frate/Command.hpp"
#include "Frate/Utils/CLI.hpp"
#include <Frate/Wizards.hpp>
#include <Frate/Utils/Validation.hpp>
#include <Frate/Constants.hpp>

namespace Wizard {
  using Utils::CLI::Prompt;
  bool Project(std::shared_ptr<Command::Project> pro){

    Prompt<std::string> prompt("Project name",pro->project_name);
    prompt.Validator(Utils::Validation::ProjectName);
    prompt.Color(Utils::CLI::Ansi::GREEN);
    while(!prompt.Run()){
    }


    pro->project_name = prompt.Get();

    Prompt<std::string> project_type("Project type",pro->project_type);
    project_type.AddOption(Command::ProjectType::EXECUTABLE);
    project_type.AddOption(Command::ProjectType::STATIC_LIBRARY);
    project_type.AddOption(Command::ProjectType::SHARED_LIBRARY);
    project_type.AddOption(Command::ProjectType::HEADER_ONLY);
    project_type.PrintValidOptions();
    project_type.Validator(Utils::Validation::ProjectType);
    project_type.Color(Utils::CLI::Ansi::GREEN);
    while(!project_type.Run()){
    }


    pro->project_type = project_type.Get();

    Prompt<std::string> language("Language",pro->lang);
    for(std::string lang: Constants::SUPPORTED_LANGUAGES){
      language.AddOption(lang);
    }
    language.PrintValidOptions();
    language.Validator(Utils::Validation::Language);
    language.Color(Utils::CLI::Ansi::GREEN);
    while(!language.Run()){
    }

    pro->lang = language.Get();

    
    if(pro->lang == "cpp"){
      Prompt<std::string> cpp_version("C++ Version",pro->lang_version);
      for(std::string version: Constants::SUPPORTED_CXX_STANDARDS){
        cpp_version.AddOption(version);
      }
      cpp_version.PrintValidOptions();
      cpp_version.Validator(Utils::Validation::CppLanguageVersion);
      cpp_version.Color(Utils::CLI::Ansi::GREEN);
      while(!cpp_version.Run()){
      }

      pro->lang_version = cpp_version.Get();

      Prompt<std::string> cpp_compiler("C++ Compiler",pro->compiler);
      for(std::string compiler: Constants::SUPPORTED_CXX_COMPILERS){
        cpp_compiler.AddOption(compiler);
      }
      cpp_compiler.PrintValidOptions();
      cpp_compiler.Validator(Utils::Validation::CppCompiler);
      cpp_compiler.Color(Utils::CLI::Ansi::GREEN);

      while(!cpp_compiler.Run()){
      }

      pro->compiler = cpp_compiler.Get();
    }
    else if(pro->lang == "c"){
      Prompt<std::string> c_version("C Version",pro->lang_version);
      for(std::string version: Constants::SUPPORTED_C_STANDARDS){
        c_version.AddOption(version);
      }
      c_version.PrintValidOptions();
      c_version.Validator(Utils::Validation::CLanguageVersion);
      c_version.Color(Utils::CLI::Ansi::GREEN);
      while(!c_version.Run()){
      }

      pro->lang_version = c_version.Get();

      pro->compiler = "gcc";

      Prompt<std::string> c_compiler("C Compiler",pro->compiler);
      for(std::string compiler: Constants::SUPPORTED_C_COMPILERS){
        c_compiler.AddOption(compiler);
      }
      c_compiler.PrintValidOptions();
      c_compiler.Validator(Utils::Validation::CCompiler);
      c_compiler.Color(Utils::CLI::Ansi::GREEN);
      while(!c_compiler.Run()){
      }

      pro->compiler = c_compiler.Get();
    }else{
      return false;
    }

    Prompt<std::string> cmake_version("CMake Version",pro->cmake_version);
    for(std::string version: Constants::SUPPORTED_CMAKE_VERSIONS){
      cmake_version.AddOption(version);
    }
    cmake_version.PrintValidOptions();
    cmake_version.Validator(Utils::Validation::CmakeVersion);
    cmake_version.Color(Utils::CLI::Ansi::GREEN);
    while(!cmake_version.Run()){
    }

    pro->cmake_version = cmake_version.Get();

    Prompt<std::string> project_version("Project Version",pro->project_version);
    project_version.Validator(Utils::Validation::ProjectVersion);
    project_version.Color(Utils::CLI::Ansi::GREEN);
    while(!project_version.Run()){
    }

    pro->project_version = project_version.Get();


    return true;
  }
}
