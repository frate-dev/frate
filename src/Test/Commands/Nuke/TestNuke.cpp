#ifdef TEST
#include <Frate/Test/Test.hpp>


namespace Tests::Command {
  bool testNuke(){
    std::cout << "Testing nuke command" << std::endl;
    if(!testNew()){
      std::cout << "Failed to create new project" << std::endl;
      return false;
    }
    std::cout << "Testing nuke command normal conditions" << std::endl;
    auto [argc,argv] = genCommand("frate nuke -d");
    ::Command::Interface *inter = new ::Command::Interface(argc,argv);
    inter->pro->project_path = std::filesystem::path(test_path);
    if(!inter->execute()){
      cleanUp(test_path);
      std::cout << "Failed to nuke project" << std::endl;
      return false;
    }
    int count = 0;
    for(std::filesystem::path p : std::filesystem::directory_iterator(test_path)){
      count++;
    }

    if(count != 0){
      std::cout << "Failed to nuke project" << std::endl;
      cleanUp(test_path);
      return false;
    }

    cleanUp(test_path);
    return true;
  }
}
#endif
