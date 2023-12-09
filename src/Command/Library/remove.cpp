#include <Frate/Command/Library.hpp>

namespace Frate::Command::Library{
  bool remove(Interface* inter, std::string link){
    std::erase_if(inter->pro->libs, [&](std::string& lib){
      return lib == link;
    });
    return inter->pro->save();
  }
}
