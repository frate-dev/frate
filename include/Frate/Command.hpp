#pragma once
#include <Frate/Frate.hpp>
#include <Frate/Package.hpp>
#include <Frate/Utils/CLI.hpp>
#include <Frate/Utils/Logging.hpp>
#include <cxxopts.hpp>
#include <nlohmann/json.hpp>
#include <sol/sol.hpp>
#include <string>

#define ENDL "\n"

namespace Frate::Command {

  namespace ProjectType {
    const std::string EXECUTABLE = "executable";
    const std::string HEADER_ONLY = "header_only";
    const std::string STATIC_LIBRARY = "static_library";
    const std::string SHARED_LIBRARY = "shared_library";

    constexpr bool validate(std::string type) {
      return type == EXECUTABLE || type == HEADER_ONLY ||
             type == STATIC_LIBRARY || type == SHARED_LIBRARY;
    }
  }; // namespace ProjectType

  std::string downloadIndex();

  json fetchIndex();

  bool updateIndex();

} // namespace Frate::Command
