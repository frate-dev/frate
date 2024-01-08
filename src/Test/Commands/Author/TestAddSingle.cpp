#ifdef TEST
#include <Frate/Project/Config.hpp>
#include <Frate/Test/Test.hpp>

namespace Tests::Command {
  bool testAddAuthorSingle(std::string name) {

    Frate::Utils::info << "Testing add author command with name: " << name
                       << std::endl;
    if (!testNew())
      return false;

    std::string command = "frate add author " + name;
    auto [failed, inter] = init(command);

    if (failed)
      return false;

    if (inter->pro->authors.size() != 1) {

      Frate::Utils::error
          << "Failed to add author project wrong number of authors"
          << std::endl;
      Frate::Utils::error << "Got: " << inter->pro->authors.size() << std::endl;
      return false;
    }

    if (inter->pro->authors[0] != name) {
      Frate::Utils::error << "Failed to add author project" << std::endl;
      Frate::Utils::error << "Got: " << inter->pro->authors[0] << std::endl;

      return false;
    }
    return true;
  }
} // namespace Tests::Command
#endif
