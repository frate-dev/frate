#include <Frate/Test/Test.hpp>


namespace Tests::Command {
  bool testRemoveAuthor(std::string name){
    cleanUp(test_path);
    info << "Testing remove author command normal conditions" << std::endl;
    if(!testNew()){
      error << "Failed to create new project" << std::endl;
      return false;
    }
    auto [failed, inter] = init("frate add author " + name);

    if(failed){
      error << "Failed add original author: " << name << std::endl;
      cleanUp(test_path);
      return false;
    }

    auto [failed_dup, inter_dup] = init("frate remove author " + name);

    if(failed_dup){
      error << "Failed to remove author: " << name << std::endl;
      cleanUp(test_path);
      return false;
    }
    return true;
  }
}
