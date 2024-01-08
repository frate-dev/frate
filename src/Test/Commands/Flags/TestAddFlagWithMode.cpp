#ifdef TEST
#include <Frate/Project/Config.hpp>
#include <Frate/Test/Test.hpp>

namespace Tests::Command {
  bool testAddFlagsWithMode() {
    std::cout << "Testing add flags command with a mode specified" << std::endl;
    if (!testNew()) {
      Frate::Utils::error << "Failed to create new project" << std::endl;
      return false;
    }

    auto [failed, inter] = init("frate add flags -m Release '-O3'");
    if (failed) {
      Frate::Utils::error << "Failed to init" << std::endl;
      return false;
    }
    if (inter->pro->modes[0].flags[1] != "-O3") {
      std::cout << inter->pro->modes[0].flags[0] << std::endl;

      Frate::Utils::error << "Failed to add flags project" << std::endl;
      return false;
    }

    return true;
  }
} // namespace Tests::Command
#endif
