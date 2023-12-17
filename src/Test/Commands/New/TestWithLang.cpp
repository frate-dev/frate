#ifdef TEST
#include <Frate/Test/Test.hpp>
#include <Frate/Utils/General.hpp>
bool Tests::Command::testNewWithLang(std::string lang){

  Frate::Utils::info << "Testing new command with lang: " << lang << std::endl;
  test_path = genTestDirectory();
  auto[failed, inter] = init("frate new test -d -l "+lang);

  if(inter->pro->lang != lang){
    
    Frate::Utils::error << "Failed to create new project : project lang not set" << std::endl;
    return false;
  }

  if(!validateProjectJson(inter)){
    
    return false;
  }
  return true;

}
#endif
