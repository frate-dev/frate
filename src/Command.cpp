#include "Command.h"
#include "iostream"
#include "string"

// namespace Command
//
//
namespace Command {
  bool handleCppProject() { return false; }

  bool handleCProject() { return false; }

  int init(std::shared_ptr<Context> ctx) {
    std::cout << "📕 Enter project name: ";
    std::cin >> ctx->name;
    std::cout << "💻 C or C++? (c/cpp): ";
    std::string lang;
    while (true) {
      std::cin >> lang;
      if (lang == "cpp") {
        handleCppProject();
        break;
      } else if (lang == "c") {
        handleCProject();
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
