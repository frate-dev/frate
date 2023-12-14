#ifdef TEST
#include "Frate/Command.hpp"
#include "Frate/Utils/General.hpp"
#include <catch2/catch_test_macros.hpp>
#include <Frate/Test/Test.hpp>


namespace Tests::Command {
  bool validateProjectJson(Frate::Command::Interface* inter){
    Frate::Utils::Error error;
    Frate::Utils::Info info;
    bool valid = true;
    info << "Testing valid project json" << std::endl;

    if (inter->pro->lang == "") {
      valid = false;
      error << "language is not cpp or c" << std::endl;
    }
    if(inter->pro->name == ""){
      valid = false;
      error << "project name is empty" << std::endl;
    }
    if(inter->pro->project_path == ""){
      valid = false;
      error << "project path is empty" << std::endl;
    }
    if(inter->pro->project_version == ""){
      valid = false;
      error << "project version is empty" << std::endl;
    }
    if(inter->pro->src_dir == ""){
      valid = false;
      error << "src path is empty" << std::endl;
    }
    if(inter->pro->include_dir == ""){
      valid = false;
      error << "include path is empty" << std::endl;
    }
    if(inter->pro->lang_version == ""){
      valid = false;
      error << "language version is empty" << std::endl;
    }
    if(inter->pro->build_dir == ""){
      valid = false;
      error << "build path is empty" << std::endl;
    }
    if(inter->pro->cmake_version == ""){
      valid = false;
      error << "cmake version is empty" << std::endl;
    }
    if(inter->pro->modes.size() == 0){
      valid = false;
      error << "modes is empty" << std::endl;
    }else{
      for(Frate::Command::Mode mode : inter->pro->modes){
        if(mode.name == ""){
          valid = false;
          error << "mode "+mode.name+" has no name" << std::endl;
        }
      }
    }
    return valid;
  }
}
#endif
