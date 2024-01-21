#include "Frate/Command/Package.hpp"
#include "Frate/Dependency.hpp"
#include <Frate/Command/CommandMode.hpp>
#include <Frate/Generators.hpp>
#include <Frate/Project/Config.hpp>
#include <Frate/Utils/CLIPrompt.hpp>
#include <Frate/Utils/General.hpp>
#include <cxxopts.hpp>

namespace Frate::Command::Packages {
  using Utils::CLI::Prompt;

  bool addPackageToMode(std::shared_ptr<Interface> inter, Dependency package,
                        std::string selected_mode) {
    Utils::info << "Adding package to mode " << selected_mode << std::endl;
    for (Project::Mode &mode : inter->pro->modes) {
      if (selected_mode == mode.name) {
        mode.dependencies.emplace_back(package);
        return true;
      }
    }
    Utils::error << "Mode " << selected_mode << " not found";
    return false;
  }

  bool add(std::shared_ptr<Interface> inter) {
    options(inter);
    bool latest = false;
    std::string mode;
    std::string query;
    std::string version;
    std::string git;
    std::string target_link;

    if (inter->args->count("mode") != 0) {
      mode = inter->args->operator[]("mode").as<std::string>();
    }
    if (inter->args->count("git") != 0) {
      git = inter->args->operator[]("git").as<std::string>();
    }
    if (inter->args->count("package-version") != 0) {
      version = inter->args->operator[]("package-version").as<std::string>();
    }
    if (inter->args->count("target_link") != 0) {
      target_link = inter->args->operator[]("target_link").as<std::string>();
    }
    if (inter->args->operator[]("latest").as<bool>()) {
      latest = true;
    }
    if (inter->args->count("args") < 1) {
      Utils::error << "No packages specified" << std::endl;
      return false;
    }

    std::vector<std::string> package_names =
        inter->args->operator[]("args").as<std::vector<std::string>>();

    if (!git.empty()) {
      std::cout << "Adding git package" << std::endl;
      std::cout << "Git: " << git << std::endl;
      std::cout << "Version: " << version << std::endl;
      Utils::info << "Adding git package" << std::endl;

      if (version.empty()) {
        Utils::error << "No version specified" << std::endl;
        Utils::error << "Please specify a version" << std::endl;
        return false;
      }

      if (target_link.empty()) {
        Prompt target_link_prompt("Specify target_link: ");
        target_link_prompt.run();
        auto target = target_link_prompt.get<std::string>();

        target_link = target;
      }

      std::cout << "Target_link: " << target_link << std::endl;
      Package package;
      package.name = package_names[0];
      // package.selected_version = version;
      package.git = git;
      package.target_link = target_link;
      package.versions.emplace_back(version);
      if (!mode.empty()) {
        if (!addPackageToMode(inter, Dependency(package, version), mode)) {
          Utils::error << "Failed to add package to mode" << std::endl;
          return false;
        }
        return true;
      }
      std::cout << "Package: " << nlohmann::json(package) << std::endl;
      inter->pro->dependencies.emplace_back(package, version);
      return true;
    }

    for (std::string package_name : package_names) {
      Utils::info << "Searching for " << package_name << std::endl;

      auto [exact, exact_package] = getExact(package_name);
      Package chosen_package;

      if (!exact) {
        Utils::error << "No exact match found" << std::endl;
        chosen_package = promptSearchResults(package_name);
      }
      else {
        Utils::info << "Exact match found" << std::endl;
        chosen_package = exact_package;
      }
      Utils::info << "Installing " << chosen_package.name << std::endl;

      std::reverse(chosen_package.versions.begin(),
                   chosen_package.versions.end());
      std::vector<std::string> versions = chosen_package.versions;

      if (!latest) {
        version = promptForVersion(chosen_package);
        // chosen_package.selected_version = version;
      }
      else {
        if (chosen_package.versions.empty()) {
          Utils::error << "No versions found" << std::endl;
          return false;
        }
        version = chosen_package.versions[0];
        // chosen_package.selected_version = version;
      }

      if (dependenciesConflict(inter->pro->dependencies, chosen_package.name)) {
        Utils::error << "Package already installed" << std::endl;
        return false;
      }

      if (!mode.empty()) {
        if (!addPackageToMode(inter, Dependency(chosen_package, version),
                              mode)) {
          Utils::error << "Failed to add package to mode" << std::endl;
          return false;
        }
      }
      inter->pro->dependencies.emplace_back(chosen_package, version);
    }

    return true;
  }
} // namespace Frate::Command::Packages
