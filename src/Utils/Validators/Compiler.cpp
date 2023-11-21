
#include "Frate/Utils/Validation.hpp"
#include <iostream>
#include <string>
#include <regex>
#include <memory>
#include <Frate/Command.hpp> 
#include <Frate/Generators.hpp> 


namespace Utils::Validation {
  bool CCompiler(std::string cccompiler){
    std::vector<std::string> supportedCCompilers = {"gcc", "clang", "msvc", "icc", "tcc", "emcc"};
    for(std::string compiler : supportedCCompilers){
      if(cccompiler == compiler){
        return true;
      }
    }
    return false;
  }
  bool CppCompiler(std::string cppcompiler){
    std::vector<std::string> supportedCppCompilers = {"g++", "clang++"};
    for(std::string compiler : supportedCppCompilers){
      if(cppcompiler == compiler){
        return true;
      }
    }
    return false;
  }
}
