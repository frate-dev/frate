#include "Frate/Utils/General.hpp"
#include <Frate/Constants.hpp>
#include <Frate/Generators.hpp>
#include <Frate/System/GitProvider.hpp>

namespace Frate::Generators::Project {
  bool downloadTemplate(std::string git_url,
                        std::shared_ptr<Command::Project> pro) {

    // std::filesystem::current_path(pro->path);
    //  Delete old template before downloading new one
    if (std::filesystem::exists(pro->path / "template")) {

      Utils::info << "Copying old template to tmp directory" << std::endl;
      path tmp_path =
          Utils::copyToTmpPath(pro->path / "template", "frate-template-");

      Utils::info << "Deleting old template" << std::endl;
      std::filesystem::remove_all(pro->path / "template");
    }

    auto git = System::GitProvider()
                   .setGitUrl(git_url)
                   .setWorkingDir(pro->path / "template" / "deez")
                   .clone();

    Utils::fetchGitArchive(git_url, "new_function_prefix",
                           pro->path / "template");

    const std::string callbacks_url =
        "https://github.com/frate-templates/frate-callbacks.git";

    Utils::fetchGitArchive(callbacks_url, "new_function_prefix",
                           pro->path / "template/scripts/frate");

    return true;
  }
} // namespace Frate::Generators::Project
