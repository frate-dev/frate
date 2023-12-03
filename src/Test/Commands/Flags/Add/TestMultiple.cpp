#ifdef TEST
#include <Frate/Test/Test.hpp>


namespace Tests::Command {
  bool testAddFlagsMultiple(){
    cleanUp(test_path);
    std::cout << "Testing add flags command" << std::endl;
    if(!testNew()){
      Frate::error << "Failed to create new project" << std::endl;
      return false;
    }
    std::cout << "Testing add flags command normal conditions" << std::endl;
    auto [failed, inter] = init("frate add flags '-g -O3'");

    if(failed){
      return false;
    }
    if(inter->pro->flags.size() != 2){
      cleanUp(test_path);
      Frate::error << "Failed to add flags project wrong number of flags" << std::endl;
      return false;
    }

    if(inter->pro->flags[0] != "-g"){
      cleanUp(test_path);
      Frate::error << "Failed to add flags project" << std::endl;
      return false;
    }
    if(inter->pro->flags[1] != "-O3"){
      cleanUp(test_path);
      Frate::error << "Failed to add flags project" << std::endl;
      return false;
    }

    cleanUp(test_path);
    return true;
  }
}
#endif
