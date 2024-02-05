#pragma once
#include <Frate/Interface.hpp>
#include <Frate/Project/Config.hpp>
#include <Frate/System/Build.hpp>
#include <Frate/System/SSH.hpp>
#include <string>

namespace Frate::Command {
  using Interface = Command::Interface;

  class BuildCommand {
  public:
    BuildCommand(std::shared_ptr<Interface> &inter);
    [[nodiscard]] std::string get_local_build_command() const; 

     [[nodiscard]] std::string get_remote_build_command() const; 

  private:
    std::shared_ptr<Interface> inter;

    [[nodiscard]] std::string get_remote_destination_path() const;

    [[nodiscard]] std::string get_executable_path() const ;
  };
} // namespace Frate::Command
