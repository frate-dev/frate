#include <Frate/Command/Toolchains.hpp>
#include <Frate/Utils/General.hpp>
#include <Frate/Command/AvailableTriples.hpp>
#include <Frate/Generators.hpp>
#include <fstream>



namespace Frate::Command::Toolchains{
  json load(){
    std::filesystem::path  path = std::string(std::getenv("HOME")) + "/.config/" + "frate/" + "toolchains.json";
    return json{};
  }
  bool list(){
    json data = load();
    for (Command::Toolchain::CompileTarget toolchain : Command::Toolchain::CompileTargets){
      std::cout << toolchain.triple << std::endl;
    }
    return true;
  }
  bool fuckit(){
    return false;
  };
  bool add(std::string toolchain, Interface* inter){
    json data = load();
    Generators::Toolchain::generateToolchain(toolchain);
    std::ofstream file;
    if (!std::filesystem::exists(inter->pro->project_path / "toolchains/")){
      std::filesystem::create_directory(inter->pro->project_path / "toolchains/");
    }
    file.open(inter->pro->project_path / "toolchains/" / (toolchain + ".cmake"));

    std::cout << "Writing toolchain file" << std::endl;
    std::string toolchain_template = Generators::Toolchain::generateToolchain(toolchain);
    std::cout << toolchain_template << std::endl;
    file << toolchain_template;
    inter->pro->toolchains.push_back(toolchain);
    return true;
  }
  bool remove(std::string toolchain_name, Interface* interface){
    json data = load();

    for (std::string toolchainProject : interface->pro->toolchains){
      if(toolchainProject == toolchain_name){
        std::erase_if(interface->pro->toolchains, [&toolchain_name](std::string &toolchain){
            return toolchain == toolchain_name;
            });
      }
    }
    std::filesystem::remove(interface->pro->project_path / "toolchains/" / (toolchain_name + ".cmake"));
    return true;
  }
}
