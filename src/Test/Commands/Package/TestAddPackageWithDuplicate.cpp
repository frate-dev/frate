#ifdef TEST
#include <Frate/Test/Test.hpp>

namespace Tests::Command {

  bool testAddPackageWithDuplicate(std::string package_name){
    info << "Testing add package command with duplicate package" << std::endl;
    if(!testNew()) return false;

    auto [failed, inter] = init("frate add package " + package_name + " -l");

    if(failed) return false;

    auto [failed_dup, inter_dup] = init("frate add package " + package_name + " -l");

    if(!failed_dup) return false;

    return true;
  }
}
#endif
