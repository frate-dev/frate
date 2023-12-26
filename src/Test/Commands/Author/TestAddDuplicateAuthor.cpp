#ifdef TEST
#include <Frate/Test/Test.hpp>

namespace Tests::Command {
  bool testAddDuplicateAuthor(std::string name) {
    Frate::Utils::info
        << "Testing add duplicate author command normal conditions"
        << std::endl;

    if (!testNew())
      return false;

    auto [failed, inter] = init("frate add author " + name);

    if (failed)
      return false;

    auto [failed_dup, inter_dup] = init("frate add author " + name);

    if (!failed_dup)
      return false;

    return true;
  }
} // namespace Tests::Command
#endif
