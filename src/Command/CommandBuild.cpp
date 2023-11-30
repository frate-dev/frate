#include "Frate/Utils/General.hpp"
#include <Frate/Command.hpp>

namespace Command {
  bool OptionsInit::Build(Interface* inter){
    inter->InitHeader();
    inter->options->parse_positional({"command"});
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("m,mode", "mode to build in", cxxopts::value<std::string>())
      ("t,target", "Target to build", cxxopts::value<std::string>())
      ("j,jobs", "Number of jobs to run", cxxopts::value<int>());
    return inter->parse();
  }
  bool Interface::build() {
    std::string target = "";
    std::string mode = "";
    int jobs = 1;
    if(this->args->count("target")){
      target = this->args->operator[]("target").as<std::string>();
    }
    if (this->args->count("mode")) {
      mode = this->args->operator[]("mode").as<std::string>();
    }
    if (this->args->count("jobs")) {
      jobs = this->args->operator[]("jobs").as<int>();
    }

    Utils::Info info;
    info << "Building project with: " << std::endl;
    info << "Target: " << target << std::endl;
    info << "Mode: " << mode << std::endl;
    info << "Jobs: " << jobs << std::endl;
    //TODO: Handle different targets

    for(Mode &m : pro->modes){
      if(m.name == mode){
        std::string workdir_cmd = "cd " + pro->project_path.string();
        std::string build_cmd = "cmake -DCMAKE_BUILD_TYPE=" + m.name + " .";
        std::string make_cmd = "make";
        if(Utils::hSystem(workdir_cmd + ";" + build_cmd) != 0){
          std::cout << "Build failed" << std::endl;
          return false;
        }else{
          std::cout << "Build success" << std::endl;
          return true;
        }
      }
    }

    std::string workdir_cmd = "cd " + pro->project_path.string();
    std::string build_cmd = "cmake -DCMAKE_BUILD_TYPE=" + pro->default_mode + " .";
    std::string make_cmd = "make -j" + std::to_string(jobs);
    if(Utils::hSystem(workdir_cmd + ";" + build_cmd) != 0){
      std::cout << "Build failed" << std::endl;
      return false;
    }else{
      if(Utils::hSystem(workdir_cmd + ";" + make_cmd) != 0){
        std::cout << "Build failed" << std::endl;
        return false;
      }
      std::cout << "Build success" << std::endl;
      return true;
    }
    return true;
  }
}
