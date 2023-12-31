#ifdef TEST
#include <Frate/Test/Test.hpp>
#include <Frate/Project.hpp>

namespace Tests::Command {
  bool testRemoveAuthor(std::string name){
    Frate::Utils::info << "Testing remove author command normal conditions" << std::endl;
    if(!testNew()){
      return false;
    }
    auto [failed, inter] = init("frate add author " + name);

    if(failed) return false;

    auto [failed_dup, inter_dup] = init("frate remove author " + name);

    if(failed_dup){
      return false;
    }
    return true;
  }
}
#endif
