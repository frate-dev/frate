#include <Frate/Generators.hpp>
#include <Frate/Constants.hpp>
#include <git2.h>
#include <git2/clone.h>


namespace Frate::Generators::Project {

  bool downloadTemplate(std::string git_url, path project_path) {
    //Delete old template before downloading new one
    if(std::filesystem::exists(project_path / "template")){

      Utils::info << "Copying old template to tmp directory" << std::endl;
      path tmp_path = Utils::copyToTmpPath(project_path / "template", "frate-template-");

      Utils::info << "Deleting old template" << std::endl;
      std::filesystem::remove_all(project_path / "template");
    }

    try{

      std::filesystem::create_directories(project_path / "template");

    }catch(...){
      Utils::error << "Error while creating template directory" << std::endl;
      return false;
    }

//     git_libgit2_init();
//     const git_error *template_clone_err;
//     Utils::info << "Downloading template" << std::endl;
//     git_repository* template_repo = NULL; 
//     git_repository* callbacks_repo = NULL;
//     Utils::info << "Cloning " << git_url << " into " 
//       << (project_path / "template").c_str() << std::endl;
// 
// 
//     int template_clone_status = git_clone(&template_repo,
//         git_url.c_str(), (project_path / "template").c_str(), NULL);
// 
// 
//     if(template_clone_status != 0){
//       template_clone_err = git_error_last();
//       Utils::error << "Clone status code: " << template_clone_status << std::endl;
//       if(template_clone_err){
//         Utils::error << "Error while cloning repository error code: " << template_clone_err->klass << std::endl;
//       }else{
//         Utils::error << "Error while cloning repository" << std::endl;
//       }
//       return false;
//     }
// 
// 
//     git_repository_free(template_repo);
// 
//     
//     Utils::info << "Cloning " << callbacks_url << " into " 
//       << (project_path / "template/frate-callbacks").c_str() << std::endl;
// 
//     const git_error *callbacks_clone_err;
//     int callbacks_clone_status= git_clone(&callbacks_repo,
//         callbacks_url.c_str(),
//         (project_path / "template/frate-callbacks").c_str(), NULL);
// 
// 
//     if(callbacks_clone_status != 0){
//       callbacks_clone_err = git_error_last();
//       Utils::error << "Clone status code: " << callbacks_clone_status << std::endl;
//       if(callbacks_clone_err){
//         Utils::error << "Error while cloning repository error code: " << callbacks_clone_err->klass << std::endl;
//       }else{
//         Utils::error << "Error while cloning repository" << std::endl;
//       }
//       return false;
//     }
// 
// 
//     git_repository_free(callbacks_repo);
// 
//     git_libgit2_shutdown();

    int status = Utils::hSystem(
        "git clone -b " +
        Constants::TEMPLATE_BRANCH + " " +
        git_url + " " + (project_path / "template").string()
        );
    if(status != 0){
      Utils::error << "Error while cloning template repo" << std::endl;
      return false;
    }

    const std::string callbacks_url = "https://github.com/frate-templates/frate-callbacks.git";
    status = Utils::hSystem(
        "git clone -b " +
        Constants::TEMPLATE_BRANCH + " " +
        std::string(callbacks_url) + " " +
        (project_path / "template/frate-callbacks").string()
        );

    if(status != 0){
      Utils::error << "Error while cloning callback repository" << std::endl;
      return false;
    }

    std::filesystem::rename(project_path / "template/frate-callbacks/scripts",
        project_path / "template/scripts");


    std::filesystem::remove_all(project_path / "template/frate-callbacks");

    try{
      std::filesystem::remove(project_path / "template/.git");
    }catch(...){
      Utils::error << "could not find .git folder in template/" << std::endl;
    }

    return true;
  }
}
