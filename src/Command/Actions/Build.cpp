#include "Frate/Utils/General.hpp"
#include <Frate/Command/Actions/Build.hpp>

namespace Frate::Command::Build {
  bool options(std::shared_ptr<Interface> inter){
    inter->InitHeader();
    inter->options->parse_positional({"command"});
    inter->options->add_options()
      ("command", "Command to run", cxxopts::value<std::string>()->default_value("help"))
      ("m,mode", "mode to build in", cxxopts::value<std::string>())
      ("t,target", "Target to build", cxxopts::value<std::string>())
      ("j,jobs", "Number of jobs to run", cxxopts::value<int>());
    return inter->parse();
  }
  bool run(std::shared_ptr<Interface> inter) {
    options(inter);
    std::string target = "";
    std::string mode = "";
    int jobs = 1;
    if(inter->args->count("target")){
      target = inter->args->operator[]("target").as<std::string>();
    }
    if (inter->args->count("mode")) {
      mode = inter->args->operator[]("mode").as<std::string>();
    }
    if (inter->args->count("jobs")) {
      jobs = inter->args->operator[]("jobs").as<int>();
    }

    Utils::Info info;
    info << "Building project with: " << std::endl;
    info << "Target: " << target << std::endl;
    info << "Mode: " << mode << std::endl;
    info << "Jobs: " << jobs << std::endl;
    //TODO: Handle different targets

    for(Mode &m : inter->pro->modes){
      if(m.name == mode){
        std::string workdir_cmd = "cd " + inter->pro->path.string();
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

    std::string workdir_cmd = "cd " + inter->pro->path.string();
    std::string build_cmd = "cmake -DCMAKE_BUILD_TYPE=" + inter->pro->default_mode + " .";
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
