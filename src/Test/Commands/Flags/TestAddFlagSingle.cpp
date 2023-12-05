#ifdef TEST
#include <Frate/Test/Test.hpp>


namespace Tests::Command {
  bool testAddFlagsSingle(){
    info  << "Testing add flags command" << std::endl;
    if(!testNew()) return false;
    info << "Testing add flags command normal conditions" << std::endl;
    auto [failed, inter] = init("frate add flags '-O3'");

    if(failed) return false;

    if(inter->pro->flags.size() != 1){
      
      error << "Failed to add flags project wrong number of flags" << std::endl;
      error << "Got: " << inter->pro->flags.size() << std::endl;
      return false;
    }

    if(inter->pro->flags[0] != "-O3"){
      error << inter->pro->flags[0] << std::endl;
      
      error << "Failed to add flags project" << std::endl;
      return false;
    }


    
    return true;


  }
}
#endif
