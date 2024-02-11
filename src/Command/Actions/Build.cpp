#include "Frate/Utils/General.hpp"
#include <Frate/Command/Actions/Build.hpp>
#include <Frate/Generators.hpp>
#include <Frate/Project/Config.hpp>

namespace Frate::Command::Build {
  bool options(std::shared_ptr<Interface> inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command"});
    // clang-format off
    inter->options->add_options()
      ("command","Command to run",cxxopts::value<std::string>()->default_value("help"))
      ("m,mode", "mode to build in", cxxopts::value<std::string>())
      ("t,target", "Target to build", cxxopts::value<std::string>())
      ("j,jobs", "Number of jobs to run", cxxopts::value<int>());
    // clang-format on
    return inter->parse();
  }

  bool run(std::shared_ptr<Interface> inter) {
    options(inter);
    std::string target = "";
    std::string mode = "";
    int jobs = 1;
    if (inter->args->count("target")) {
      target = inter->args->operator[]("target").as<std::string>();
    }
    if (inter->args->count("mode")) {
      mode = inter->args->operator[]("mode").as<std::string>();
    }
    if (inter->args->count("jobs")) {
      jobs = inter->args->operator[]("jobs").as<int>();
    }

    inter->pro->load();


    Utils::info << "Building project with: " << std::endl;
    Utils::info << "Target: " << target << std::endl;
    Utils::info << "Mode: " << mode << std::endl;
    Utils::info << "Jobs: " << jobs << std::endl;
    // TODO: Handle different targets

    inter->local->setBuildMode(mode == "" ? inter->pro->default_mode : mode);
    inter->local->setRequestedJobs(jobs);

    inter->pro->current_template.refresh(inter->pro, inter->local);

    Utils::verbose << "Build command" << inter->local->getBuildCommand() << std::endl;

    if (Utils::hSystem(inter->local->getBuildCommand()) != 0) {
      Utils::error << "Build failed" << std::endl;
      return false;
    }
    Utils::info << "Build success" << std::endl;
    return true;
  }
} // namespace Frate::Command::Build
