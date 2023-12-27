#include "Frate/Utils/General.hpp"
#include <Frate/Constants.hpp>
#include <Frate/Generators.hpp>

namespace Frate::Generators::Project {
  bool downloadTemplate(std::string git_url, path project_path) {
    // Delete old template before downloading new one
    if (std::filesystem::exists(project_path / "template")) {

      Utils::info << "Copying old template to tmp directory" << std::endl;
      path tmp_path =
          Utils::copyToTmpPath(project_path / "template", "frate-template-");

      Utils::info << "Deleting old template" << std::endl;
      std::filesystem::remove_all(project_path / "template");
    }

    try {

      std::filesystem::create_directories(project_path / "template");

    } catch (...) {
      Utils::error << "Error while creating template directory" << std::endl;
      return false;
    }

    // Clones main template repo
    int status = Utils::hSystem("git clone --depth=1 -b " +
                                Constants::TEMPLATE_BRANCH + " " + git_url +
                                " " + (project_path / "template").string());
    if (status != 0) {
      Utils::error << "Error while cloning template repo" << std::endl;
      return false;
    }

    const std::string callbacks_url =
        "https://github.com/frate-templates/frate-callbacks.git";

    status =
        Utils::hSystem("git clone --depth=1 -b " + Constants::TEMPLATE_BRANCH +
                       " " + std::string(callbacks_url) + " " +
                       (project_path / "template/frate-callbacks").string());

    // Removes .git folders
    std::filesystem::remove_all(project_path / "template/frate-callbacks/.git");
    std::filesystem::remove_all(project_path / "template/.git");

    if (status != 0) {
      Utils::error << "Error while cloning callback repository" << std::endl;
      return false;
    }

    std::filesystem::rename(project_path / "template/frate-callbacks/scripts",
                            project_path / "template/scripts");

    std::filesystem::remove_all(project_path / "template/frate-callbacks");

    return true;
  }
} // namespace Frate::Generators::Project
