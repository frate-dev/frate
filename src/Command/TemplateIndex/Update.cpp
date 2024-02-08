#include <Frate/Command/TemplateIndex.hpp>


namespace Frate::Command::TemplateIndex {

  bool update(std::shared_ptr<Frate::Command::Interface> inter) {
    try{
      inter->templates->updateIndex();
    } catch (std::exception &e) {
      Utils::error << e.what() << std::endl;
      return false;
    }
    return true;
  }
}
