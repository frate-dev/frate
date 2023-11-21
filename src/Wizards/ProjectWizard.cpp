#include "Frate/Command.hpp"
#include "Frate/Utils/CLI.hpp"
#include <Frate/Wizards.hpp>
#include <Frate/Utils/Validation.hpp>


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
    language.AddOption("cpp");
    language.AddOption("c");
    language.PrintValidOptions();
    language.Validator(Utils::Validation::Language);
    language.Color(Utils::CLI::Ansi::GREEN);
    while(!language.Run()){
    }

    pro->lang = language.Get();

    
    if(pro->lang == "cpp"){
      Prompt<std::string> cpp_version("C++ Version",pro->lang_version);
      cpp_version.AddOption("98");
      cpp_version.AddOption("11");
      cpp_version.AddOption("1x");
      cpp_version.AddOption("14");
      cpp_version.AddOption("17");
      cpp_version.AddOption("2a");
      cpp_version.AddOption("20");
      cpp_version.AddOption("23");
      cpp_version.AddOption("26");
      cpp_version.PrintValidOptions();
      cpp_version.Validator(Utils::Validation::CppLanguageVersion);
      cpp_version.Color(Utils::CLI::Ansi::GREEN);
      while(!cpp_version.Run()){
      }

      pro->lang_version = cpp_version.Get();

      Prompt<std::string> cpp_compiler("C++ Compiler",pro->compiler);
      cpp_compiler.AddOption("g++");
      cpp_compiler.AddOption("clang++");
      cpp_compiler.PrintValidOptions();
      cpp_compiler.Validator(Utils::Validation::CppCompiler);
      cpp_compiler.Color(Utils::CLI::Ansi::GREEN);

      while(!cpp_compiler.Run()){
      }

      pro->compiler = cpp_compiler.Get();
    }
    else if(pro->lang == "c"){
      Prompt<std::string> c_version("C Version",pro->lang_version);
      c_version.AddOption("89");
      c_version.AddOption("90");
      c_version.AddOption("95");
      c_version.AddOption("99");
      c_version.AddOption("11");
      c_version.AddOption("1x");
      c_version.AddOption("18");
      c_version.AddOption("23");
      c_version.AddOption("26");
      c_version.PrintValidOptions();
      c_version.Validator(Utils::Validation::CLanguageVersion);
      c_version.Color(Utils::CLI::Ansi::GREEN);
      while(!c_version.Run()){
      }

      pro->lang_version = c_version.Get();

      pro->compiler = "gcc";

      Prompt<std::string> c_compiler("C Compiler",pro->compiler);
      c_compiler.AddOption("gcc");
      c_compiler.AddOption("clang");
      c_compiler.AddOption("icc");
      c_compiler.AddOption("msvc");
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
    cmake_version.AddOption("3.0");
    cmake_version.AddOption("3.1");
    cmake_version.AddOption("3.2");
    cmake_version.AddOption("3.3");
    cmake_version.AddOption("3.4");
    cmake_version.AddOption("3.5");
    cmake_version.AddOption("3.6");
    cmake_version.AddOption("3.7");
    cmake_version.AddOption("3.8");
    cmake_version.AddOption("3.9");
    cmake_version.AddOption("3.10");
    cmake_version.AddOption("3.11");
    cmake_version.AddOption("3.12");
    cmake_version.AddOption("3.13");
    cmake_version.AddOption("3.14");
    cmake_version.AddOption("3.15");
    cmake_version.AddOption("3.16");
    cmake_version.AddOption("3.17");
    cmake_version.AddOption("3.18");
    cmake_version.AddOption("3.19");
    cmake_version.AddOption("3.20");
    cmake_version.AddOption("3.21");
    cmake_version.AddOption("3.22");
    cmake_version.AddOption("3.23");
    cmake_version.AddOption("3.24");
    cmake_version.AddOption("3.25");
    cmake_version.AddOption("3.26");
    cmake_version.AddOption("3.27");
    cmake_version.AddOption("3.28");
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
