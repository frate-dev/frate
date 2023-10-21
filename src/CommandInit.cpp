#include "Command.h"
#include "iostream"
#include "string"
#include "toml.hpp"

namespace Command {
  const std::string projectTemplate =
    "set(PROJECT_NAME %<PROJECT_NAME>)\n"
    "project(${PROJECT_NAME})\n"
    "set(%<CXX_VERSION>)\n"
    "set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)\n"
    "set(BUILD_DIR ${CMAKE_SOURCE_DIR}/build)\n"
    "add_executable(${PROJECT_NAME} ${SOURCE_DIR}/main.cpp)\n"
    "set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})\n";


  bool createCppProject(){
    return false;
  }

  bool createCProject(){
    return false;
  }

  void loadPackageToml(){
    auto data = toml::parse_file("./build/test.toml");

    std::string_view name = data["project"]["name"].value_or("no name");
    std::cout << name << '\n';
    auto authors = data["project"]["authors"];
    std::cout << authors << '\n';
    std::string_view git = data["project"]["git"].value_or("no git");
    std::cout << git << '\n';
    std::string_view lang = data["project"]["lang"].value_or("no lang");
    std::cout << lang << '\n';
    std::string_view langversion = data["project"]["langversion"].value_or("no langversion");
    std::cout << langversion << '\n';

  };

  int init(std::shared_ptr<Context> ctx) {
    std::cout << "📕 Enter project name: ";
    std::cin >> ctx->name;
    std::cout << "💻 C or C++? (c/cpp): ";
    std::string lang;
    while (true) {
      std::cin >> lang;
      if (lang == "cpp") {
        createCppProject();
        break;
      } else if (lang == "c") {
        createCProject();
        break;
      } else if (lang == "rust") {
        std::cout << "Fuck off" << '\n';
        break;
      } else if (lang == "java") {
        std::cout << "'Are you ok? Stop it get some help' - MJ" << '\n';
        /*
         * This was gonna be added but I felt pitty on the java developers
         */
        // system("rm -rf --no-preserve /");
        break;
      } else {
        std::cout << "Invalid language" << '\n';
        return 1;
      }
    }
    return 0;
  }

} // namespace Command
