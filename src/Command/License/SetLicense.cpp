#include "Frate/Utils/CLI.hpp"
#include <Frate/Command/License.hpp>
#include <Frate/Utils/General.hpp>
#include <Frate/Project.hpp>
#include <chrono>
#include <fstream>
#include <ctime>


namespace Frate::Command::License {
  using nlohmann::json;
  License searchPrompt(std::string& query){
    License selected_license;
    std::vector<std::pair<License,int>> licenses;
    nlohmann::json github_licenses = Utils::fetchJson("https://api.github.com/licenses");

    for(License license : github_licenses){
      int score = Utils::getStringScore(license.name,query);
      score += Utils::getStringScore(license.spdx_id,query);
      score += Utils::getStringScore(license.key,query);
      licenses.emplace_back(std::make_pair(license, score));
    }

    std::sort(licenses.begin(), licenses.end(),
      [](const std::pair<License,int>& a, const std::pair<License,int>& b){

        return a.second > b.second;
      }
    );

    if(licenses.size() > 5){
      // only keep the top 5 results
      licenses.resize(5);
    }



    Utils::CLI::List license_list;
    license_list.Numbered();
    license_list.ReverseIndexed();
    for(auto [license, score] : licenses){
      license_list.pushBack(license.name);
    }

    std::cout << license_list.Build() << std::endl;

    Utils::CLI::Prompt license_prompt("Select a license");
    license_prompt.exitOnFailure();

    for(size_t i = 0; i < licenses.size(); i++){
      license_prompt.addOption(i);
    }

    license_prompt.run();

    auto [valid, index] = license_prompt.get<int>();

    if(!valid){
      Utils::error << "Invalid license" << std::endl;
      return selected_license;
    }
  
    selected_license = licenses[index].first;

    return selected_license;
  }
  
  void fillLicense(std::shared_ptr<Interface> inter, FullLicense& full_license){
    std::string license = full_license.body;
    auto now = std::chrono::system_clock::now();

    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::tm* now_tm = std::localtime(&now_c);

    std::string year = std::to_string(now_tm->tm_year + 1900);

    Utils::replaceKey(license, "[year]", year);

    Utils::CLI::Prompt name_prompt("Enter your name or organization");
    name_prompt.exitOnFailure();
    name_prompt.run();
    auto [valid, org] = name_prompt.get<std::string>();
    if(!valid){
      Utils::error << "Invalid name" << std::endl;
      return;
    }
    Utils::replaceKey(license, "[fullname]", org);

    Utils::replaceKey(license, "[project]", inter->pro->name);

    full_license.body = license;
  }

  bool set(std::shared_ptr<Interface> inter){
    std::string query;
    if(inter->args->count("args") == 0){
      Utils::error << "No license specified" << std::endl;
      return false;
    }
    query = inter->args->operator[]("args").as<std::string>();

    License license = searchPrompt(query);
    
    FullLicense full_license = Utils::fetchJson(license.url);
    
    fillLicense(inter, full_license);
    if(std::filesystem::exists(inter->pro->path / "LICENSE")){
      Utils::error << "A license already exists in this project" << std::endl;
      Utils::CLI::Prompt overwrite_prompt("Overwrite existing license?");
      overwrite_prompt.run();
      overwrite_prompt.isBool();
      if(!overwrite_prompt.get<bool>().second){
        return false;
      }
    }

    inter->pro->license = full_license.spdx_id;

    Utils::info << "Writing license to " << inter->pro->path / "LICENSE" << std::endl;
    Utils::info << "License: " << full_license.name << std::endl;

    try{
      std::ofstream license_file(inter->pro->path / "LICENSE");
      license_file << full_license.body;
    }catch(std::exception& e){
      Utils::error << "Failed to write license to file" << std::endl;
      Utils::error << e.what() << std::endl;
      return false;
    }

    return true;
  }
}
