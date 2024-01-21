#include "Frate/Utils/CLIPrompt.hpp"
#include <Frate/Utils/CLIColors.hpp>
#include <Frate/Constants.hpp>
#include <Frate/Project/Config.hpp>
#include <Frate/Utils/Validation.hpp>
#include <Frate/Wizards.hpp>

namespace Frate::Wizard {
  using Utils::CLI::Prompt;

  bool Project(std::shared_ptr<Project::Config> pro) {

    Prompt prompt("Project name", pro->name);
    prompt.setValidator(Utils::Validation::ProjectName);
    prompt.maxLength(30);
    prompt.setColor(Utils::CLI::Ansi::GREEN);
    prompt.run();

    auto project_name = prompt.get<std::string>();
    pro->name = project_name;

    Prompt author_prompt("Author");
    prompt.setColor(Utils::CLI::Ansi::GREEN);
    author_prompt.run();

    auto author = author_prompt.get<std::string>();
    pro->authors.emplace_back(author);

    Prompt description_prompt("Description", pro->description);
    description_prompt.maxLength(255);
    description_prompt.setColor(Utils::CLI::Ansi::GREEN);
    description_prompt.run();

    pro->description = description_prompt.get<std::string>();


    Prompt language("Language", pro->lang);
    for (std::string lang : Constants::SUPPORTED_LANGUAGES) {
      language.addOption(lang);
    }
    language.printValidOptions();
    language.setValidator(Utils::Validation::Language);
    language.setColor(Utils::CLI::Ansi::GREEN);
    language.run();

    pro->lang = language.get<std::string>();

    if (pro->lang == "cpp") {
      Prompt cpp_version("C++ Version", pro->lang_version);
      for (std::string version : Constants::SUPPORTED_CXX_STANDARDS) {
        cpp_version.addOption(version);
      }

      cpp_version.printValidOptions();
      cpp_version.setValidator(Utils::Validation::CppLanguageVersion);
      cpp_version.setColor(Utils::CLI::Ansi::GREEN);
      cpp_version.run();

      pro->lang_version = cpp_version.get<std::string>();

      Prompt cpp_compiler("C++ Compiler", pro->compiler);
      for (std::string compiler : Constants::SUPPORTED_CXX_COMPILERS) {
        cpp_compiler.addOption(compiler);
      }
      cpp_compiler.printValidOptions();
      cpp_compiler.setValidator(Utils::Validation::CppCompiler);
      cpp_compiler.setColor(Utils::CLI::Ansi::GREEN);

      cpp_compiler.run();

      pro->compiler = cpp_compiler.get<std::string>();
    }
    else if (pro->lang == "c") {
      Prompt c_version("C Version", pro->lang_version);

      for (std::string version : Constants::SUPPORTED_C_STANDARDS) {
        c_version.addOption(version);
      }
      c_version.printValidOptions()
          .setValidator(Utils::Validation::CLanguageVersion)
          .setColor(Utils::CLI::Ansi::GREEN)
          .run();

      pro->lang_version = c_version.get<std::string>();

      pro->compiler = "gcc";

      Prompt c_compiler("C Compiler", pro->compiler);
      for (std::string compiler : Constants::SUPPORTED_C_COMPILERS) {
        c_compiler.addOption(compiler);
      }
      c_compiler.printValidOptions()
          .setValidator(Utils::Validation::CCompiler)
          .setColor(Utils::CLI::Ansi::GREEN)
          .run();

      pro->compiler = c_compiler.get<std::string>();
    }
    else {
      return false;
    }

    Prompt cmake_version_prompt("CMake Version", pro->cmake_version);
    for (std::string version : Constants::SUPPORTED_CMAKE_VERSIONS) {
      cmake_version_prompt.addOption(version);
    }
    cmake_version_prompt.printValidOptions()
        .setValidator(Utils::Validation::CmakeVersion)
        .setColor(Utils::CLI::Ansi::GREEN)
        .run();

    pro->cmake_version = cmake_version_prompt.get<std::string>();

    Prompt project_version_prompt("Project Version", pro->version);
    project_version_prompt.setValidator(Utils::Validation::ProjectVersion);
    project_version_prompt.setColor(Utils::CLI::Ansi::GREEN);
    project_version_prompt.run();

    pro->version = project_version_prompt.get<std::string>();

    return true;
  }
} // namespace Frate::Wizard
