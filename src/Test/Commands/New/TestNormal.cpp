#ifdef TEST
#include <Frate/Test/Test.hpp>
bool Tests::Command::testNew(){
  Frate::Utils::info << "Testing new command normal conditions" << std::endl;

  test_path = genTestDirectory();
  auto [failed, inter] = init("frate new test -d");
  if(failed){
    Frate::Utils::error << "Failed to create new project" << std::endl;
    return false;
  }

  return true;

}
#endif
