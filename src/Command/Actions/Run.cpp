#include <Frate/Command/Actions/Run.hpp>
#include <Frate/Utils/General.hpp>
namespace Command::Run {
using std::filesystem::create_directories;

bool run(Interface* inter) {
  std::cout << "Running project: " << inter->pro->project_name << std::endl;

  const std::string work_dir_cmd = "cd " +inter->pro->project_path.string();
  const std::string cmake_cmd = work_dir_cmd + ";cmake .";
  const std::string make_cmd = work_dir_cmd + ";make";
  const std::string run_cmd = work_dir_cmd + ";./build/" +inter->pro->project_name;
  std::cout << "project_name " <<inter->pro->project_name << ENDL;

  /*
   * Creating build directory and building project
   */
  try{
    create_directories(inter->pro->project_path / "build");
  }catch(std::exception& e){
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
} // namespace Command
