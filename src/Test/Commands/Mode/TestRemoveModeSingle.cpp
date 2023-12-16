#ifdef TEST
#include <Frate/Test/Test.hpp>

bool Tests::Command::testRemoveMode(std::string mode){
  Frate::Utils::info << "Testing remove mode command normal conditions" << std::endl;
  if(!testNew()) return false;

  auto [failed, inter] = init("frate add mode " + mode);

  if(failed) return false;

  auto [failed_remove, inter_remove] = init("frate remove mode " + mode);

  if(failed_remove){
    return false;
  }
  return true;
}
#endif
