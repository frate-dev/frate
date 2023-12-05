#ifdef TEST
#include <Frate/Test/Test.hpp>
bool Tests::Command::testNew(){
  info << "Testing new command normal conditions" << std::endl;

  test_path = genTestDirectory();
  auto [failed, inter] = init("frate new test -d");
  if(failed){
    Frate::error << "Failed to create new project" << std::endl;
    return false;
  }

  return true;

}
#endif
