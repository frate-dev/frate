#include <Frate/Command/Template.hpp>

namespace Frate::Command::Template {
  bool remove(std::shared_ptr<Frate::Command::Interface> inter) {
    std::string template_name;
    bool all;


    if (inter->args->count("args")) {
      template_name = inter->args->operator[]("args").as<std::string>();
    }
    if (inter->args->count("all")) {
      all = inter->args->operator[]("all").as<bool>();
    }
    
    Utils::info << "Removing template " << template_name << std::endl;
    Utils::info << "All: " << all << std::endl;

    if (all) {
      try{
        inter->templates->deleteAll();
        return true;
      } catch (std::exception &e) {
        Utils::error << e.what() << std::endl;
        return false;
      }
    } else {
      if(template_name.empty()) {
        Utils::error << "No template name given" << std::endl;
        return false;
      }
      try{
        inter->templates->uninstallAll(template_name);
        return true;
      } catch (std::exception &e) {
        Utils::error << e.what() << std::endl;
        return false;
      }
    }

    return false;
  }
}
