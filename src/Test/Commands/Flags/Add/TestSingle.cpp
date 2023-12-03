#ifdef TEST
#include <Frate/Test/Test.hpp>


namespace Tests::Command {
  bool testAddFlagsSingle(){
    cleanUp(test_path);
    info  << "Testing add flags command" << std::endl;
    if(!testNew()){
      error << "Failed to create new project" << std::endl;
      return false;
    }
    info << "Testing add flags command normal conditions" << std::endl;
    auto [failed, inter] = init("frate add flags '-O3'");

    if(failed){
      error << "Failed to init" << std::endl;
      cleanUp(test_path);
      return false;
    }

    if(inter->pro->flags.size() != 1){
      cleanUp(test_path);
      error << "Failed to add flags project wrong number of flags" << std::endl;
      error << "Got: " << inter->pro->flags.size() << std::endl;
      return false;
    }

    if(inter->pro->flags[0] != "-O3"){
      error << inter->pro->flags[0] << std::endl;
      cleanUp(test_path);
      error << "Failed to add flags project" << std::endl;
      return false;
    }


    cleanUp(test_path);
    return true;


  }
}
#endif
