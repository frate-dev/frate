#include <Frate/Generators.hpp>
#include <inja.hpp>


namespace Frate::Generators::Project {
  using inja::Environment;
  bool refreshTemplate(Environment &env, std::shared_ptr<Command::Project> pro) {
    Utils::info << "Refreshing template" << std::endl;
    std::vector<path> paths_to_refresh{
      pro->path / "template/CMakeLists.txt.inja",
    };

    for(const path& current_p: paths_to_refresh){
      std::string rendered_file = env.render_file(current_p, pro->toJson());
      std::string new_file = current_p.string();
      new_file = new_file.replace(new_file.find(".inja"), 5, "");
      Utils::replaceKey(new_file, "template/", "");
      std::ofstream file;
      try{
        file.open(new_file);
      }catch(...){
        Utils::error << "Error while opening file: " << new_file << std::endl;
        return false;
      }
      file << rendered_file;
      file.close();
    }
    return true;
  }
}
