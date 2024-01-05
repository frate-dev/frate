#include "Frate/System/Build.hpp"
#include <Frate/Generators.hpp>
#include <Frate/Interface.hpp>
#include <Frate/Project.hpp>
#include <Frate/System/Watcher.hpp>
#include <Frate/Utils/General.hpp>
#include <uv.h>

namespace Frate::Command::UvWatch {

  bool options(std::shared_ptr<Interface> inter) {
    inter->InitHeader();
    inter->options->parse_positional({"command"});
    // clang-format off
    inter->options->add_options()
      ("c,command","Command to run",cxxopts::value<std::string>()->default_value("help"))
      ("e,execute","Execute command",cxxopts::value<std::string>()->default_value("false"))
      ("r,remote","Build server to use",cxxopts::value<bool>()->default_value("false"));
    // clang-format on
    return inter->parse();
  }

  bool runCommand(std::shared_ptr<Interface> inter) {

    BuildCommand build_command(inter);
    std::string command = build_command.get_local_build_command();

    bool build_server = inter->args->operator[]("remote").as<bool>();
    if (build_server) {
      command = build_command.get_remote_build_command();
    }

    if (Utils::hSystem(command) != 0) {
      std::cout << "Error running command: " << command << std::endl;
      exit(1);
    };

    return true;
  }

  bool watch(std::shared_ptr<Interface> inter) {
    options(inter);
    inter->pro->load();
    Generators::Project::refresh(inter);

    System::UVDirectoryWatcher uv_watcher;
    uv_watcher.watch(inter->pro->path / inter->pro->src_dir,
                     [inter](std::string filename) {
                       std::cout << "File changed: " << filename << std::endl;
                       runCommand(inter);
                     });
    uv_watcher.run();

    return true;
  };
} // namespace Frate::Command::UvWatch
