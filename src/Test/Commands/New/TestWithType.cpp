#ifdef TEST
#include "Frate/Utils/General.hpp"
#include <Frate/Test/Test.hpp>
bool Tests::Command::testNewWithType(std::string type){
  info << "Testing new command with type: " << type << std::endl;
  cleanUp(test_path);
  auto[failed,inter] = init("frate new -t " + type);

  if(failed){
    return false;
  }

  if(inter->pro->project_type != type){
    cleanUp(test_path);
    error << "Failed to create new project : project type: " << type << " not set" << std::endl;
    return false;
  }

  return true;

}
#endif
