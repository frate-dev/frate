#ifdef TEST
#include <Frate/Test/Test.hpp>


namespace Tests::Command {
  bool testAddAuthorMultiple(std::vector<std::string> names) {
    cleanUp(test_path);

    info << "Testing add author command with names: " << std::endl;
    for(auto name : names){
      info << name << std::endl;
    }

    if (!testNew()) {
      error << "Failed to create new project" << std::endl;
      return false;
    }

    std::string command = "frate add author ";

    for (auto name : names) {
      command += name + " ";
    }
    auto [failed, inter] = init(command);

    if (failed) {
      error << "Failed to init" << std::endl;
      cleanUp(test_path);
      return false;
    }

    if (inter->pro->authors.size() != names.size()) {
      cleanUp(test_path);
      error << "Failed to add author project wrong number of authors" << std::endl;
      error << "Got: " << inter->pro->authors.size() << std::endl;
      return false;
    }
    for(auto name : names){
      bool found = false;
      for(auto author : inter->pro->authors){
        if(author == name){
          found = true;
        }
      }
      if(!found){
        error << "Failed to add author project" << std::endl;
        error << "Got: " << inter->pro->authors[0] << std::endl;
        cleanUp(test_path);
        return false;
      }
    }


    cleanUp(test_path);
    return true;
  }
}
#endif
