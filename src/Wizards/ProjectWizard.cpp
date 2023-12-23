#include "Frate/Interface.hpp"
#include "Frate/Utils/CLI.hpp"
#include <Frate/Wizards.hpp>
#include <Frate/Utils/Validation.hpp>
#include <Frate/Constants.hpp>
#include <Frate/Project.hpp>

namespace Frate::Wizard {
  using Utils::CLI::Prompt;
  bool Project(std::shared_ptr<Command::Project> pro){

    Prompt prompt("Project name",pro->name);
    prompt.setValidator(Utils::Validation::ProjectName);
    prompt.maxLength(30);
    prompt.setColor(Utils::CLI::Ansi::GREEN);
    prompt.run();

    {
      auto [valid, project_name] = prompt.get<std::string>();

      if(valid){
        pro->name = project_name;
      }else{
        return false;
      }
    }
    
    Prompt author_prompt("Author");
    prompt.setColor(Utils::CLI::Ansi::GREEN);
    author_prompt.run();

    {
      auto [valid, author] = author_prompt.get<std::string>();
      if(valid){
        pro->authors.push_back(author);
      }else{
        return false;
      }
    }



    Prompt description_prompt("Description",pro->description);
    description_prompt.maxLength(255);
    description_prompt.setColor(Utils::CLI::Ansi::GREEN);
    description_prompt.run();

    {
      auto [valid, project_description] = description_prompt.get<std::string>();
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
      language.addOption(lang);
    }
    language.printValidOptions();
    language.setValidator(Utils::Validation::Language);
    language.setColor(Utils::CLI::Ansi::GREEN);
    language.run();

    {
      auto [valid,lang] = language.get<std::string>();
      if(valid){
        pro->lang = lang;
      }else{
        return false;
      }
    }


    
    if(pro->lang == "cpp"){
      Prompt cpp_version("C++ Version",pro->lang_version);
      for(std::string version: Constants::SUPPORTED_CXX_STANDARDS){
        cpp_version.addOption(version);
      }

      cpp_version.printValidOptions();
      cpp_version.setValidator(Utils::Validation::CppLanguageVersion);
      cpp_version.setColor(Utils::CLI::Ansi::GREEN);
      cpp_version.run();

      {
        auto [valid,lang_version] = cpp_version.get<std::string>();
        if(valid){
          pro->lang_version = lang_version;
        }else{
          return false;
        }
      }

      Prompt cpp_compiler("C++ Compiler",pro->compiler);
      for(std::string compiler: Constants::SUPPORTED_CXX_COMPILERS){
        cpp_compiler.addOption(compiler);
      }
      cpp_compiler.printValidOptions();
      cpp_compiler.setValidator(Utils::Validation::CppCompiler);
      cpp_compiler.setColor(Utils::CLI::Ansi::GREEN);

      cpp_compiler.run();

      {
        auto [valid,compiler] = cpp_compiler.get<std::string>();
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
        c_version.addOption(version);
      }
      c_version.printValidOptions();
      c_version.setValidator(Utils::Validation::CLanguageVersion);
      c_version.setColor(Utils::CLI::Ansi::GREEN);
      c_version.run();

      {
        auto [valid,lang_version] = c_version.get<std::string>();
        if(valid){
          pro->lang_version = lang_version;
        }else{
          return false;
        }
      }

      pro->compiler = "gcc";

      Prompt c_compiler("C Compiler",pro->compiler);
      for(std::string compiler: Constants::SUPPORTED_C_COMPILERS){
        c_compiler.addOption(compiler);
      }
      c_compiler.printValidOptions();
      c_compiler.setValidator(Utils::Validation::CCompiler);
      c_compiler.setColor(Utils::CLI::Ansi::GREEN);
      c_compiler.run();

      {
        auto [valid,compiler] = c_compiler.get<std::string>();
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
      cmake_version_prompt.addOption(version);
    }
    cmake_version_prompt.printValidOptions();
    cmake_version_prompt.setValidator(Utils::Validation::CmakeVersion);
    cmake_version_prompt.setColor(Utils::CLI::Ansi::GREEN);
    cmake_version_prompt.run();

    {
      auto [valid,cmake_version] = cmake_version_prompt.get<std::string>();
      if(valid){
        pro->cmake_version = cmake_version;
      }else{
        return false;
      }
    }


    Prompt project_version_prompt("Project Version",pro->version);
    project_version_prompt.setValidator(Utils::Validation::ProjectVersion);
    project_version_prompt.setColor(Utils::CLI::Ansi::GREEN);
    project_version_prompt.run();
    {
      auto [valid,project_version] = project_version_prompt.get<std::string>();
      if(valid){
        pro->version = project_version;
      }else{
        return false;
      }
    }
    return true;
  }
}
