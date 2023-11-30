#ifdef TEST
#include <Frate/Test/Test.hpp>


namespace Tests::Command {
  bool testFlagsSingle(){
    std::cout << "Testing add flags command" << std::endl;
    if(!testNew()){
      std::cout << "Failed to create new project" << std::endl;
      return false;
    }
    std::cout << "Testing add flags command normal conditions" << std::endl;
    auto [argc,argv] = genCommand("frate add flags '-O3'");
    ::Command::Interface *inter = new ::Command::Interface(argc,argv);
    inter->pro->project_path = std::filesystem::path(test_path);
    if(!inter->execute()){
      cleanUp(test_path);
      std::cout << "Failed to add flags project" << std::endl;
      return false;
    }

    if(inter->pro->flags[0] != "-'O3'"){
      std::cout << inter->pro->flags[0] << std::endl;
      cleanUp(test_path);
      std::cout << "Failed to add flags project" << std::endl;
      return false;
    }


    cleanUp(test_path);
    return true;


  }
}
#endif
