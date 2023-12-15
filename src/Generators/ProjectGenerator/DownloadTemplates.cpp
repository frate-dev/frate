#include <Frate/Generators.hpp>
#include <git2.h>
#include <git2/clone.h>


namespace Frate::Generators::Project {

  bool downloadTemplate(std::string git_url, path project_path) {
    //Delete old template before downloading new one
    if(std::filesystem::exists(project_path / "template")){
      Frate::info << "Deleting old template" << std::endl;
      std::filesystem::remove_all(project_path / "template");
    }

    std::filesystem::create_directories(project_path / "template");


    info << "Downloading template" << std::endl;
    git_repository* template_repo = NULL; 
    git_repository* callbacks_repo = NULL;
    Frate::info << "Cloning " << git_url << " into " 
      << (project_path / "template").c_str() << std::endl;


    int template_clone_status = git_clone(&template_repo,
        git_url.c_str(), (project_path / "template").c_str(), NULL);


    if(template_clone_status != 0){
      Frate::error << "Error while cloning repository" << std::endl;
      return false;
    }


    git_repository_free(template_repo);

    const std::string callbacks_url = "https://github.com/frate-templates/frate-callbacks.git";

    Frate::info << "Cloning " << callbacks_url << " into " 
      << (project_path / "template/frate-callbacks").c_str() << std::endl;


    int callbacks_clone_status= git_clone(&callbacks_repo,
        callbacks_url.c_str(),
        (project_path / "template/frate-callbacks").c_str(), NULL);


    if(callbacks_clone_status != 0){
      Frate::error << "Error while cloning repository" << std::endl;
      return false;
    }


    git_repository_free(callbacks_repo);

    std::filesystem::rename(project_path / "template/frate-callbacks/scripts",
        project_path / "template/scripts");

    std::filesystem::remove_all(project_path / "template/frate-callbacks");

    try{
      std::filesystem::remove(project_path / "template/.git");
    }catch(...){
      Frate::error << "could not find .git folder in template/" << std::endl;
    }

    return true;
  }



}
