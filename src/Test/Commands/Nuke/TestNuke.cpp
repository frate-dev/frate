#ifdef TEST
#include <Frate/Test/Test.hpp>


namespace Tests::Command {
  void TestNuke(){
    std::cout << "Testing nuke command" << std::endl;
    if(!testNew()){
      std::cout << "Failed to create new project" << std::endl;
      return;
    }
    std::cout << "Testing nuke command normal conditions" << std::endl;
    auto [argc,argv] = genCommand("frate nuke -d");
    ::Command::Interface *inter = new ::Command::Interface(argc,argv);
    inter->pro->project_path = std::filesystem::path(test_path);
    if(!inter->execute()){
      cleanUp(test_path);
      std::cout << "Failed to nuke project" << std::endl;
      return;
    }



    cleanUp(test_path);


  }
}
#endif
