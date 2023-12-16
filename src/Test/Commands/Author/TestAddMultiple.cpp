#ifdef TEST
#include <Frate/Test/Test.hpp>


namespace Tests::Command {
  bool testAddAuthorMultiple(std::vector<std::string> names) {
    

    Frate::Utils::info << "Testing add author command with names: " << std::endl;
    for(auto name : names){
      Frate::Utils::info << name << std::endl;
    }

    if (!testNew()) {
      Frate::Utils::error << "Failed to create new project" << std::endl;
      return false;
    }

    std::string command = "frate add author ";

    for (auto name : names) {
      command += name + " ";
    }
    auto [failed, inter] = init(command);

    if (failed) {
      Frate::Utils::error << "Failed to init" << std::endl;
      
      return false;
    }

    if (inter->pro->authors.size() != names.size()) {
      
      Frate::Utils::error << "Failed to add author project wrong number of authors" << std::endl;
      Frate::Utils::error << "Got: " << inter->pro->authors.size() << std::endl;
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
        Frate::Utils::error << "Failed to add author project" << std::endl;
        Frate::Utils::error << "Got: " << inter->pro->authors[0] << std::endl;
        
        return false;
      }
    }


    
    return true;
  }
}
#endif
