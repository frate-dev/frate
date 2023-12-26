#ifdef TEST
#include <Frate/Test/Test.hpp>

bool Tests::Command::testAddDuplicateMode(std::string mode_name) {
  Frate::Utils::info << "Testing add mode command with duplicate mode"
                     << std::endl;
  if (!testNew())
    return false;

  auto [failed, inter] = init("frate add mode " + mode_name);

  if (failed)
    return false;

  auto [failed_dup, inter_dup] = init("frate add mode " + mode_name);

  // If the second init fails, then the test passes
  if (!failed_dup)
    return false;

  return true;
}

#endif
