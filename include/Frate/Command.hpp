#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <sol/sol.hpp>
#include <cxxopts.hpp>
#include <Frate/Utils/CLI.hpp>
#include <Frate/Utils/Logging.hpp>
#include <Frate/Frate.hpp>
#include <Frate/Package.hpp>

#define ENDL "\n"

namespace Frate::Command {




  namespace ProjectType {
    const std::string EXECUTABLE = "executable";
    const std::string HEADER_ONLY = "header_only";
    const std::string STATIC_LIBRARY = "static_library";
    const std::string SHARED_LIBRARY = "shared_library";
    constexpr bool validate(std::string type) {
      return type == EXECUTABLE 
        || type == HEADER_ONLY 
        || type == STATIC_LIBRARY 
        || type == SHARED_LIBRARY;
    }
  };

  std::string downloadIndex();

  json fetchIndex();

  bool updateIndex();

}
