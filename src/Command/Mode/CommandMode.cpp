#include <Frate/Command/CommandMode.hpp>
#include <Frate/Command/Package.hpp>
#include <Frate/Generators.hpp>
#include <Frate/Interface.hpp>
#include <Frate/Project/Config.hpp>
#include <Frate/Project/Mode.hpp>

namespace Frate::Command::ModeCommands {
  bool addFlags(std::shared_ptr<Interface> inter, std::string mode) {
    // std::vector<std::string> flags =
    // inter->args->operator[]("args").as<std::vector<std::string>>();
    std::vector<std::string> flags = inter->args->unmatched();
    for (Project::Mode m : inter->pro->modes) {
      if (m.name == mode) {
        for (std::string f : flags) {
          m.flags.emplace_back(f);
        }
      }
    }
    return true;
  }

  [[deprecated("Use addPackage in the package module")]] bool
  addPackages(std::shared_ptr<Interface> inter, std::string mode) {
    std::vector<std::string> dependencies =
        inter->args->operator[]("args").as<std::vector<std::string>>();
    for (std::string dep_str : dependencies) {
      auto [found, new_package] = Packages::searchWithPrompt(dep_str, false);
      if (found) {
        // Push the found package to the dependencies
        for (Project::Mode &m : inter->pro->modes) {
          if (m.name == mode) {
            m.dependencies.emplace_back(new_package);
          }
        }
      }
      else {
        std::cout << "Package " << dep_str << " not found" << std::endl;
      }
    }
    return true;
  }

  [[deprecated("Use removePackage in the package module")]] bool
  removePackages(std::shared_ptr<Interface> inter, std::string mode) {
    std::vector<std::string> dependencies =
        inter->args->operator[]("args").as<std::vector<std::string>>();
    for (std::string dep : dependencies) {
      for (Project::Mode &m : inter->pro->modes) {
        if (m.name == mode) {
          std::erase_if(m.dependencies,
                        [&dep](auto &d) { return d.name == dep; });
        }
      }
    }
    return true;
  }
}; // namespace Frate::Command::ModeCommands
