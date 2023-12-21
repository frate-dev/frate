#include "Frate/Utils/General.hpp"
#include <Frate/Command/Actions/Build.hpp>
#include <Frate/Generators.hpp>
#include <Frate/Project.hpp>

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

    inter->pro->load();
    Generators::Project::refresh(inter->pro);

    Utils::info << "Building project with: " << std::endl;
    Utils::info << "Target: " << target << std::endl;
    Utils::info << "Mode: " << mode << std::endl;
    Utils::info << "Jobs: " << jobs << std::endl;
    //TODO: Handle different targets
    
    for(Mode &m : inter->pro->modes){
      if(m.name == mode){
        std::string workdir_cmd = "cd " + inter->pro->path.string();
        Utils::replaceKey(inter->pro->build_command, "\n", ";");
        std::string full_build_cmd = inter->pro->build_command;
        if(Utils::hSystem(workdir_cmd + ";" + full_build_cmd) != 0){
          Utils::error << "Build failed" << std::endl;
          return false;
        }else{
          Utils::info << "Build success" << std::endl;
          return true;
        }
      }
    }
    
    std::string workdir_cmd = "cd " + inter->pro->path.string();
    Utils::replaceKey(inter->pro->build_command, "\n", ";");
    std::string full_build_cmd = workdir_cmd + ";" + inter->pro->build_command;

    if(Utils::hSystem(full_build_cmd) != 0){
      Utils::error << "Build failed" << std::endl;
      return false;
    }
    Utils::info << "Build success" << std::endl;
    return true;
  }
}
