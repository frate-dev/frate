#include <Frate/Command/Toolchains.hpp>
#include <Frate/Utils/General.hpp>
#include <Frate/Command/AvailableTriples.hpp>
#include <Frate/Generators.hpp>
#include <filesystem>
#include <fstream>
#include <Frate/Project.hpp>



namespace Frate::Command::Toolchains{
  json load(){
    std::filesystem::path  path = std::string(std::getenv("HOME")) + "/.config/" + "frate/" + "toolchains.json";
    return json{};
  }
  bool list(std::shared_ptr<Interface> inter){
    json data = load();
    for (Command::Toolchain::CompileTarget toolchain : Command::Toolchain::CompileTargets){
      std::cout << toolchain.triple << std::endl;
    }
    return true;
  }
  bool add(std::shared_ptr<Interface> inter){
    Prompt toolchain_prompt("Toolchain");
    for (Command::Toolchain::CompileTarget toolchain : Command::Toolchain::CompileTargets){
      toolchain_prompt.addOption(toolchain.triple);
    }
    toolchain_prompt.printValidOptions().run();
    auto [valid, toolchain] = toolchain_prompt.get<std::string>();
    if(!valid){
      std::cout << "Invalid toolchain" << std::endl;
      return false;
    }
    json data = load();
    Generators::Toolchain::generateToolchain(toolchain);
    std::ofstream file;
    if (!std::filesystem::exists(inter->pro->path / "toolchains/")){
      try{
        std::filesystem::create_directory(inter->pro->path / "toolchains/");
      }catch(...){
        Utils::error << "Error while creating toolchains directory" << std::endl;
        return false;
      }
    }
    file.open(inter->pro->path / "toolchains/" / (toolchain + ".cmake"));

    std::cout << "Writing toolchain file" << std::endl;
    std::string toolchain_template = Generators::Toolchain::generateToolchain(toolchain);
    std::cout << toolchain_template << std::endl;
    file << toolchain_template;
    inter->pro->toolchains.push_back(toolchain);
    return true;
  }
  bool remove(std::string toolchain_name, std::shared_ptr<Interface> interface){
    json data = load();

    for (std::string toolchainProject : interface->pro->toolchains){
      if(toolchainProject == toolchain_name){
        std::erase_if(interface->pro->toolchains, [&toolchain_name](std::string &toolchain){
            return toolchain == toolchain_name;
            });
      }
    }

    std::filesystem::path tmp_path = Utils::copyToTmpPath(interface->pro->path,"toolchains-remove-frate-");
    std::filesystem::remove(interface->pro->path / "toolchains/" / (toolchain_name + ".cmake"));
    return true;
  }
}
