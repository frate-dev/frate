#ifdef TEST
#include <Frate/Test/Test.hpp>
bool Tests::Command::testNew(){
  std::cout << "Testing new command normal conditions" << std::endl;
  init(test_path);
  auto [argc,argv] = genCommand("frate new test -d");
  ::Command::Interface *inter = new ::Command::Interface(argc,argv);

  inter->pro->project_path = std::filesystem::path(test_path);


  if(!inter->execute()){
    cleanUp(test_path);
    std::cout << "Failed to create new project" << std::endl;
    return false;
  }
  if(!validateProjectJson(inter)){
    cleanUp(test_path);
    return false;
  }
  return true;

}
#endif
