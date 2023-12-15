#include <Frate/Generators.hpp>
#include <fstream>
#include <inja.hpp>


namespace Frate::Generators::Project {
  using inja::Environment;


  bool loadTemplateConfig(std::shared_ptr<Command::Project> pro){


    try{

      std::filesystem::copy(
          pro->path / "template/default.json",
          pro->path / "frate-project.json"
          );

    }catch(...){

      error << "Error while copying frate-project.json" << std::endl;
      return false;

    }
  

    std::ifstream file;


    try{

      file.open(pro->path / "frate-project.json");

    }catch(...){

      error << "Error while opening file: " << pro->path / "frate-project.json" << std::endl;
      return false;

    }

    json current_j = pro->toJson();
    json j = json::parse(file);

    j.merge_patch(current_j);

    pro->fromJson(j);


    return true;
  }
}
