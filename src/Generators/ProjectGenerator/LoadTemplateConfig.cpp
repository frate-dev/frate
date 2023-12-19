#include <Frate/Generators.hpp>
#include <fstream>
#include <inja.hpp>


namespace Frate::Generators::Project {
  using inja::Environment;


  bool loadTemplateConfig(std::shared_ptr<Command::Project> pro){


//     try{
// 
//       std::filesystem::copy(
//           pro->path / "template/default.json",
//           pro->path / "frate-project.json"
//           );
// 
//     }catch(...){
// 
//       Utils::error << "Error while copying frate-project.json" << std::endl;
//       return false;
// 
//     }
//   
// 
//     std::ifstream file;
// 
// 
//     try{
// 
//       file.open(pro->path / "frate-project.json");
// 
//     }catch(...){
// 
//       Utils::error << "Error while opening file: " << pro->path / "frate-project.json" << std::endl;
//       return false;
// 
//     }

    std::ifstream template_config_file;

    try{

      template_config_file.open(pro->path / "template/template.json");

    }catch(...){

      Utils::error << "Error while opening file: " << pro->path / "template/template.json" << std::endl;
      return false;

    }


    json j = json::parse(template_config_file);

    Command::Template template_config = j;

    //Utils::verbose << "Template config: " << nlohmann::json(template_config).dump(2) << std::endl;

    pro->fromTemplate(template_config);

//     Utils::info << "Merging frate-project.json" << std::endl;
//     
//     current_j.merge_patch(j);
// 
//     pro = std::make_shared<Command::Project>(current_j);


    return true;
  }
}
