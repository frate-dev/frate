#include <Frate/Command.hpp>
#include <Frate/Command/AvailableTriples.hpp>
#include <Frate/Utils/General.hpp>
#include <inja.hpp>
#include <nlohmann/json.hpp>
#include <string>

namespace Frate::Generators::Toolchain {
  using nlohmann::json;
  std::string generateToolchain(std::string toolchain) {
    Command::Toolchain::CompileTarget compiletarget;
    for (Command::Toolchain::CompileTarget comptarget :
        Command::Toolchain::CompileTargets) {
      if (comptarget.triple == toolchain) {
        std::string processor = Utils::split(comptarget.triple, '-')[0];
        compiletarget = {comptarget.os, comptarget.triple, processor};
      }
    }
    std::string toolchain_template = R"(
set(CMAKE_SYSTEM_NAME {{ os }})
set(CMAKE_SYSTEM_PROCESSOR {{ processor }})
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER_TARGET {{ triple }})
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_TARGET {{ triple }})
set(CMAKE_LINKER ldd)
    )";
    return inja::render(
        toolchain_template,
        json{
          {"os", compiletarget.os},
          {"processor", compiletarget.processor},
          {"triple", compiletarget.triple}
        });
    }
}
