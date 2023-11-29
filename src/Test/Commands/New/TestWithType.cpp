#ifdef TEST
#include "Frate/Utils/General.hpp"
#include <Frate/Test/Test.hpp>
bool Tests::Command::testNewWithType(std::string type){
  ::Utils::Info info;
  ::Utils::Error error;
  info << "Testing new command with type: " << type << std::endl;
  init(test_path);
  auto [argc,argv] = genCommand("frate new test -d -t "+type);
  ::Command::Interface *inter = new ::Command::Interface(argc,argv);

  inter->pro->project_path = std::filesystem::path(test_path);

  if(!inter->execute()){
    cleanUp(test_path);
    error << "Failed to create new project : Could not execute" << std::endl;
    return false;
  }
  if(inter->pro->project_type != type){
    cleanUp(test_path);
    error << "Failed to create new project : project type not set" << std::endl;
    return false;
  }

  if(!validateProjectJson(inter)){
    cleanUp(test_path);
    return false;
  }
  return true;

}
#endif
