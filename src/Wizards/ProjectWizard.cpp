#include "Frate/Command.hpp"
#include "Frate/Utils/CLI.hpp"
#include <Frate/Wizards.hpp>
#include <Frate/Utils/Validation.hpp>
#include <Frate/Constants.hpp>

namespace Frate::Wizard {
  using Utils::CLI::Prompt;
  bool Project(std::shared_ptr<Command::Project> pro){

    Prompt prompt("Project name",pro->name);
    prompt.Validator(Utils::Validation::ProjectName);
    prompt.MaxLength(30);
    prompt.Color(Utils::CLI::Ansi::GREEN);
    prompt.Run();

    {
      auto [valid, project_name] = prompt.Get<std::string>();

      if(valid){
        pro->name = project_name;
      }else{
        return false;
      }
    }
    Prompt description_prompt("Description",pro->description);
    description_prompt.MaxLength(255);
    description_prompt.Color(Utils::CLI::Ansi::GREEN);
    description_prompt.Run();

    {
      auto [valid, project_description] = description_prompt.Get<std::string>();
      if(valid){
        pro->description = project_description;
      }else{
        return false;
      }
    }


//     Prompt project_type_prompt("Project type",pro->project_type);
//     project_type_prompt.AddOption(Command::ProjectType::EXECUTABLE);
//     project_type_prompt.AddOption(Command::ProjectType::STATIC_LIBRARY);
//     project_type_prompt.AddOption(Command::ProjectType::SHARED_LIBRARY);
//     project_type_prompt.AddOption(Command::ProjectType::HEADER_ONLY);
//     project_type_prompt.PrintValidOptions();
//     project_type_prompt.Validator(Utils::Validation::ProjectType);
//     project_type_prompt.Color(Utils::CLI::Ansi::GREEN);
//     project_type_prompt.Run();
// 
//     {
//       auto [valid,project_type]= project_type_prompt.Get<std::string>();
//       if(valid){
//         pro->project_type = project_type;
//       }else{
//         return false;
//       }
//     }

    Prompt language("Language",pro->lang);
    for(std::string lang: Constants::SUPPORTED_LANGUAGES){
      language.AddOption(lang);
    }
    language.PrintValidOptions();
    language.Validator(Utils::Validation::Language);
    language.Color(Utils::CLI::Ansi::GREEN);
    language.Run();

    {
      auto [valid,lang] = language.Get<std::string>();
      if(valid){
        pro->lang = lang;
      }else{
        return false;
      }
    }


    
    if(pro->lang == "cpp"){
      Prompt cpp_version("C++ Version",pro->lang_version);
      for(std::string version: Constants::SUPPORTED_CXX_STANDARDS){
        cpp_version.AddOption(version);
      }

      cpp_version.PrintValidOptions();
      cpp_version.Validator(Utils::Validation::CppLanguageVersion);
      cpp_version.Color(Utils::CLI::Ansi::GREEN);
      cpp_version.Run();

      {
        auto [valid,lang_version] = cpp_version.Get<std::string>();
        if(valid){
          pro->lang_version = lang_version;
        }else{
          return false;
        }
      }

      Prompt cpp_compiler("C++ Compiler",pro->compiler);
      for(std::string compiler: Constants::SUPPORTED_CXX_COMPILERS){
        cpp_compiler.AddOption(compiler);
      }
      cpp_compiler.PrintValidOptions();
      cpp_compiler.Validator(Utils::Validation::CppCompiler);
      cpp_compiler.Color(Utils::CLI::Ansi::GREEN);

      cpp_compiler.Run();

      {
        auto [valid,compiler] = cpp_compiler.Get<std::string>();
        if(valid){
          pro->compiler = compiler;
        }else{
          return false;
        }
      }

    }
    else if(pro->lang == "c"){
      Prompt c_version("C Version",pro->lang_version);
      for(std::string version: Constants::SUPPORTED_C_STANDARDS){
        c_version.AddOption(version);
      }
      c_version.PrintValidOptions();
      c_version.Validator(Utils::Validation::CLanguageVersion);
      c_version.Color(Utils::CLI::Ansi::GREEN);
      c_version.Run();

      {
        auto [valid,lang_version] = c_version.Get<std::string>();
        if(valid){
          pro->lang_version = lang_version;
        }else{
          return false;
        }
      }

      pro->compiler = "gcc";

      Prompt c_compiler("C Compiler",pro->compiler);
      for(std::string compiler: Constants::SUPPORTED_C_COMPILERS){
        c_compiler.AddOption(compiler);
      }
      c_compiler.PrintValidOptions();
      c_compiler.Validator(Utils::Validation::CCompiler);
      c_compiler.Color(Utils::CLI::Ansi::GREEN);
      c_compiler.Run();

      {
        auto [valid,compiler] = c_compiler.Get<std::string>();
        if(valid){
          pro->compiler = compiler;
        }else{
          return false;
        }
      }

    }else{
      return false;
    }

    Prompt cmake_version_prompt("CMake Version",pro->cmake_version);
    for(std::string version: Constants::SUPPORTED_CMAKE_VERSIONS){
      cmake_version_prompt.AddOption(version);
    }
    cmake_version_prompt.PrintValidOptions();
    cmake_version_prompt.Validator(Utils::Validation::CmakeVersion);
    cmake_version_prompt.Color(Utils::CLI::Ansi::GREEN);
    cmake_version_prompt.Run();

    {
      auto [valid,cmake_version] = cmake_version_prompt.Get<std::string>();
      if(valid){
        pro->cmake_version = cmake_version;
      }else{
        return false;
      }
    }


    Prompt project_version_prompt("Project Version",pro->version);
    project_version_prompt.Validator(Utils::Validation::ProjectVersion);
    project_version_prompt.Color(Utils::CLI::Ansi::GREEN);
    project_version_prompt.Run();
    {
      auto [valid,project_version] = project_version_prompt.Get<std::string>();
      if(valid){
        pro->version = project_version;
      }else{
        return false;
      }
    }
    return true;
  }
}
