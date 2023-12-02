#ifdef TEST
#include <Frate/Test/Test.hpp>


namespace Tests::Command {
  bool testAddFlagsWithMode(){
    std::cout << "Testing add flags command" << std::endl;
    if(!testNew()){
      Frate::error << "Failed to create new project" << std::endl;
      return false;
    }
    std::cout << "Testing add flags command normal conditions" << std::endl;

    auto [failed, inter] = init("frate add flags -m Release '-O3'");
    if (failed) {
      Frate::error << "Failed to init" << std::endl;
      return false;
    }
    if (inter->pro->modes[0].flags[1] != "-O3") {
      std::cout << inter->pro->modes[0].flags[0] << std::endl;
      cleanUp(test_path);
      Frate::error << "Failed to add flags project" << std::endl;
      return false;
    }

    cleanUp(test_path);

    return true;

  }
}
#endif
