#ifdef TEST
#include <Frate/Test/Test.hpp>


namespace Tests::Command {
  bool testNuke(){
    std::cout << "Testing nuke command" << std::endl;
    cleanUp(test_path);
    if(!testNew()){
      Frate::error << "Failed to create new project" << std::endl;
      return false;
    }
    std::cout << "Testing nuke command normal conditions" << std::endl;
    auto [failed, inter] = init("frate nuke",false);

    if(failed){
      Frate::error << "Failed to init nuke command" << std::endl;
      return false;
    }

    int count = 0;
    for(std::filesystem::path p : std::filesystem::directory_iterator(test_path)){
      count++;
    }

    if(count != 0){
      Frate::error << "Failed to nuke project had: " << count << " files left" << std::endl;
      cleanUp(test_path);
      return false;
    }

    cleanUp(test_path);
    return true;
  }
}
#endif
