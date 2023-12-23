#ifdef TEST
#include <Frate/Test/Test.hpp>

namespace Tests::Command {

  bool testAddPackageWithDuplicate(std::string package_name){
    Frate::Utils::info << "Testing add package command with duplicate package" << std::endl;
    if(!testNew()) return false;

    auto [failed, inter] = init("frate add package " + package_name + " -l");

    if(failed){ 
      Frate::Utils::error << "Failed to add package" << std::endl;
      return false;
    }

    auto [failed_dup, inter_dup] = init("frate add package " + package_name + " -l");

    if(!failed_dup){ 
      Frate::Utils::error << "Failed to detect duplicate package" << std::endl;
      return false;
    }

    return true;
  }
}
#endif
