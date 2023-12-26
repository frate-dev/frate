#ifdef TEST
#include <Frate/Test/Test.hpp>

namespace Tests::Command {
  bool testNuke() {
    std::cout << "Testing nuke command" << std::endl;

    if (!testNew()) {
      Frate::Utils::error << "Failed to create new project" << std::endl;
      return false;
    }
    std::cout << "Testing nuke command normal conditions" << std::endl;
    auto [failed, inter] = init("frate nuke", false);

    if (failed) {
      Frate::Utils::error << "Failed to init nuke command" << std::endl;
      return false;
    }

    int count = 0;
    for (std::filesystem::path p :
         std::filesystem::directory_iterator(test_path)) {
      count++;
    }

    if (count != 0) {
      Frate::Utils::error << "Failed to nuke project had: " << count
                          << " files left" << std::endl;

      return false;
    }

    return true;
  }
} // namespace Tests::Command
#endif
