#pragma once
#include <Frate/System/SSH.hpp>
#include <Frate/System/Build.hpp>
#include <Frate/Interface.hpp>
#include <Frate/Project.hpp>
#include <string>

namespace Frate::Command  {
  using  Interface = Command::Interface;

  class BuildCommand {
  public:
    BuildCommand(std::shared_ptr<Interface> &inter)   {
      this->inter = inter;
    }

    [[nodiscard]] std::string get_local_build_command() const {
#ifdef DEBUG
      return "cd build && cmake . && make && " + get_executable_path();
#else
      return "cmake . && make && " + get_executable_path();
#endif
    }

    [[nodiscard]] std::string get_remote_build_command() const {
      std::string remote_dest_path = get_remote_destination_path();
      SSHConnection ssh( inter->config.getBuildServer());
      std::string build_command =
          "cd " + remote_dest_path + " && cmake . && make -j $(nproc)";
      return ssh.createRsyncCommand(inter->pro->path.string(),
                                    remote_dest_path) +
             " && " + ssh.createRemoteCommand(build_command);
    }

  private:
    std::shared_ptr<Interface> inter;

    [[nodiscard]] std::string get_remote_destination_path() const {
      char *dest_path = std::getenv("REMOTE_DEST_PATH");
      return (dest_path != nullptr) ? dest_path : ("/tmp/" + inter->pro->name);
    }

    [[nodiscard]] std::string get_executable_path() const {
      return inter->pro->path.string() + "/" + inter->pro->build_dir + "/" +
             inter->pro->name;
    }
  };
}
