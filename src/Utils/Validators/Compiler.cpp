
#include "Frate/Utils/Validation.hpp"
#include <string>
#include <Frate/Interface.hpp> 
#include <Frate/Generators.hpp> 
#include <Frate/Constants.hpp>


namespace Frate::Utils::Validation {
  bool CCompiler(std::string cccompiler){
    for(std::string compiler : Constants::SUPPORTED_C_COMPILERS){
      if(cccompiler == compiler){
        return true;
      }
    }
    return false;
  }
  bool CppCompiler(std::string cppcompiler){
    for(std::string compiler : Constants::SUPPORTED_CXX_COMPILERS){
      if(cppcompiler == compiler){
        return true;
      }
    }
    return false;
  }
}
