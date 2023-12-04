#ifdef TEST
#include <Frate/Test/Test.hpp>
bool Tests::Command::testNew(){
  info << "Testing new command normal conditions" << std::endl;

  test_path = genTestDirectory();
  auto [failed, inter] = init("frate new test -d");
  if(failed) return false;

  return true;

}
#endif
