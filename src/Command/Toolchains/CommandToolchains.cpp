#include <Frate/Command/Toolchains.hpp>
#include <Frate/Utils/General.hpp>
#include <Frate/Command/AvailableTriples.hpp>
#include <Frate/Generators.hpp>



namespace Command::Toolchains{
  json load(){
    std::filesystem::path  path = std::string(std::getenv("HOME")) + "/.config/" + "frate/" + "toolchains.json";
    return json{};
  }
  bool list(){
    json data = load();
    for (std::string toolchain : Command::Toolchain::available_triples){
      std::cout << toolchain << std::endl;
    }
    return true;
  }
  bool add(std::string toolchain, Interface* inter){
    json data = load();
    inter->pro->toolchains.push_back(toolchain);
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
    return true;
  }
  bool remove(std::string toolchain_name, Interface* inter){
    json data = load();

    for (std::string toolchainProject : inter->pro->toolchains){
      if(toolchainProject == toolchain_name){
        std::erase_if(inter->pro->toolchains, [&toolchain_name](std::string &toolchain){
            return toolchain == toolchain_name;
            });
      }
    }
    std::filesystem::remove(inter->pro->project_path / "toolchains/" / (toolchain_name + ".cmake"));
    return true;
  }
}
