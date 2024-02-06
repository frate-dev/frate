#include <Frate/Command/Actions/Run.hpp>
#include <Frate/Generators.hpp>
#include <Frate/Project/Config.hpp>
#include <Frate/Utils/General.hpp>

namespace Frate::Command::Run {
  using std::filesystem::create_directories;

  bool options(std::shared_ptr<Interface> inter) {
    // TODO: Add options
    return true;
  }

  bool run(std::shared_ptr<Interface> inter) {
    options(inter);
    inter->pro->load();
    inter->pro->current_template.refresh(inter->pro, inter->local);
    std::cout << "Running project: " << inter->pro->name << std::endl;

    const std::string work_dir_cmd = "cd " + inter->pro->path.string();
    const std::string cmake_cmd = work_dir_cmd + ";cmake .";
    const std::string make_cmd = work_dir_cmd + ";make";
    const std::string run_cmd = work_dir_cmd + ";./build/" + inter->pro->name;
    std::cout << "project_name " << inter->pro->name << std::endl;

    /*
     * Creating build directory and building project
     */
    try {
      create_directories(inter->pro->path / "build");
    } catch (std::exception &e) {
      Utils::error << "Error while creating build directory" << std::endl;
      Utils::debug(e.what());
      return false;
    }

    int success = Utils::hSystem(cmake_cmd);
    if (success != 0) {
      std::cout << "Error running cmake" << std::endl;
      return false;
    }

    success = Utils::hSystem(make_cmd);
    if (success != 0) {
      std::cout << "Error running make" << std::endl;
      return false;
    }

    /*
     * Running project
     */

    success = Utils::hSystem(run_cmd);
    if (success != 0) {
      std::cout << "Error running project" << std::endl;
      return false;
    }

    return true;
  }
} // namespace Frate::Command::Run
