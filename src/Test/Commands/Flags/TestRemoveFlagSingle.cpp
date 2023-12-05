#ifdef TEST
#include <Frate/Test/Test.hpp>
bool Tests::Command::testRemoveFlagsSingle(std::string name){
  info << "Testing remove flags command normal conditions" << std::endl;
  if(!testNew()){
    return false;
  }
  auto [failed, inter] = init("frate add flags '" + name + "'");

  if(failed) return false;

  auto [failed_remove, inter_remove] = init("frate remove flags '" + name + "'");

  if(failed_remove){
    return false;
  }
  


  return true;
}




#endif
