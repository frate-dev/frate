#ifdef TEST
#include <Frate/Test/Test.hpp>
bool Tests::Command::testNew(){
  cleanUp(test_path);
  std::cout << "Testing new command normal conditions" << std::endl;
  auto [failed, inter] = init("frate new test -d");
  if(failed) return false;

  return true;

}
#endif
