#ifdef TEST
#include <Frate/Test/Test.hpp>
#include <Frate/Project.hpp>

namespace Tests::Command {
  bool testAddFlagsMultiple(){
    std::cout << "Testing add flags multiple command" << std::endl;

    if(!testNew()) return false;


    auto [failed, inter] = init("frate add flags '-g -O3'");

    if(failed) return false;

    if(inter->pro->flags.size() != 2){
      
      Frate::Utils::error << "Failed to add flags project wrong number of flags" << std::endl;
      return false;
    }

    if(inter->pro->flags[0] != "-g"){
      
      Frate::Utils::error << "Failed to add flags project" << std::endl;
      return false;
    }
    if(inter->pro->flags[1] != "-O3"){
      
      Frate::Utils::error << "Failed to add flags project" << std::endl;
      return false;
    }

    return true;
  }
}
#endif
