#ifdef TEST
#include <Frate/Test/Test.hpp>
#include <Frate/Utils/General.hpp>
bool Tests::Command::testNewWithLang(std::string lang){
  ::Utils::Info info;
  ::Utils::Error error;

  info << "Testing new command with lang: " << lang << std::endl;
  init(test_path);
  auto [argc,argv] = genCommand("frate new test -d -l "+lang);
  ::Command::Interface *inter = new ::Command::Interface(argc,argv);

  inter->pro->project_path = std::filesystem::path(test_path);


  if(!inter->execute()){
    cleanUp(test_path);
    error << "Failed to create new project" << std::endl;
    return false;
  }

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
