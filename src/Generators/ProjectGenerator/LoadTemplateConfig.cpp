#include <Frate/Generators.hpp>
#include <fstream>
#include <inja.hpp>
#include <Frate/Template.hpp>
#include <Frate/Project.hpp>


namespace Frate::Generators::Project {
  bool loadTemplateConfig(std::shared_ptr<Command::Project> pro){

    std::ifstream template_config_file;

    try{

      template_config_file.open(pro->path / "template/template.json");

    }catch(...){

      Utils::error << "Error while opening file: " << pro->path / "template/template.json" << std::endl;
      return false;

    }


    json j = json::parse(template_config_file);

    Command::Template template_config = j;
    pro->fromTemplate(template_config);
    return true;
  }
}
