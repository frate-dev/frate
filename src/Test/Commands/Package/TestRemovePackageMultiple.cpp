#ifdef TEST
#include <Frate/Package.hpp>
#include <Frate/Project.hpp>
#include <Frate/Test/Test.hpp>

namespace Tests::Command {
  bool testRemovePackageMultiple(std::vector<std::string> packages) {

    if (!testAddPackageMultiple(packages))
      return false;

    std::string command = "frate remove p ";
    for (auto package : packages) {
      command += package + " ";
    }

    auto [failed, inter] = init(command);

    if (failed)
      return false;

    nlohmann::json config = *inter->pro;

    for (auto &dep : config["dependencies"]) {
      for (auto &p : packages) {
        if (dep["name"] == p) {
          Frate::Utils::error << "Failed to remove package : found " << p
                              << " in frate-project.json" << std::endl;
          return false;
        }
      }
    }

    return true;
  }
} // namespace Tests::Command
#endif
