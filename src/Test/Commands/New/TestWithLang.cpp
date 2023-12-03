#ifdef TEST
#include <Frate/Test/Test.hpp>
#include <Frate/Utils/General.hpp>
bool Tests::Command::testNewWithLang(std::string lang){

  info << "Testing new command with lang: " << lang << std::endl;
  cleanUp(test_path);
  auto[failed, inter] = init("frate new test -d -l "+lang);

  if(inter->pro->lang != lang){
    cleanUp(test_path);
    error << "Failed to create new project : project lang not set" << std::endl;
    return false;
  }

  if(!validateProjectJson(inter)){
    cleanUp(test_path);
    return false;
  }
  return true;

}
#endif
