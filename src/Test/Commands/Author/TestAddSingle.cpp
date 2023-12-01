#include <Frate/Test/Test.hpp>


namespace Tests::Command {
  bool testAddAuthorSingle(std::string name){
    cleanUp(test_path);

    info << "Testing add author command with name: " << name << std::endl;
    if(!testNew()){
      error << "Failed to create new project" << std::endl;
      return false;
    }

    std::string command = "frate add author " + name;
    auto [failed, inter] = init(command);

    if(failed){
      error << "Failed to init" << std::endl;
      cleanUp(test_path);
      return false;
    }

    if(inter->pro->authors.size() != 1){
      cleanUp(test_path);
      error << "Failed to add author project wrong number of authors" << std::endl;
      error << "Got: " << inter->pro->authors.size() << std::endl;
      return false;
    }

    if(inter->pro->authors[0] != name){
      error << "Failed to add author project" << std::endl;
      error << "Got: " << inter->pro->authors[0] << std::endl;
      cleanUp(test_path);
      return false;
    }
    return true;
  }
}
