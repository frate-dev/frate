#include "Frate/Utils/General.hpp"
#include <Frate/Generators.hpp>
#include <Frate/Project.hpp>
#include <inja.hpp>

namespace Frate::Generators::Project {
  using inja::Environment;

  bool refreshTemplate(Environment &env,
                       std::shared_ptr<Command::Project> pro) {
    Utils::info << "Refreshing template" << std::endl;

    const std::string render_suffix = ".inja";

    std::vector<path> paths_to_refresh{
        pro->template_path / "CMakeLists.txt.inja",
    };

    for (const path &current_p : paths_to_refresh) {
      std::string rendered_file = env.render_file(current_p, *pro);
      std::string current_file = current_p.string();
      std::string relative_path =
          current_file.substr(current_file.find(pro->template_path.string()) +
                                  pro->template_path.string().size() + 1,
                              current_file.size());

      std::filesystem::path new_file = pro->path / relative_path;
      new_file = new_file.replace_extension("");

      Utils::info << "Rendering file: " << relative_path << std::endl;

      if (!std::filesystem::exists(new_file)) {

        std::filesystem::copy_file(
            current_p, new_file,
            std::filesystem::copy_options::overwrite_existing);
      }

      std::ofstream file;
      try {
        file.open(new_file);
      } catch (...) {
        Utils::error << "Error while opening file: " << new_file << std::endl;
        throw std::runtime_error("Error while opening file: " +
                                 new_file.string());
      }
      file.clear();

      file << rendered_file;
    }
    return true;
  }
} // namespace Frate::Generators::Project
