#ifdef TEST
#include <Frate/Test/Test.hpp>


namespace Tests::Command {
  bool testAddFlagsSingle(){
    if(!testNew()) return false;
    Utils::info << "Testing add flags command normal conditions" << std::endl;
    auto [failed, inter] = init("frate add flags '-O3'");

    if(failed) return false;

    if(inter->pro->flags.size() != 1){
      
      Utils::error << "Failed to add flags project wrong number of flags" << std::endl;
      Utils::error << "Got: " << inter->pro->flags.size() << std::endl;
      return false;
    }

    if(inter->pro->flags[0] != "-O3"){
      Utils::error << inter->pro->flags[0] << std::endl;
      
      Utils::error << "Failed to add flags project" << std::endl;
      return false;
    }


    
    return true;


  }
}
#endif
