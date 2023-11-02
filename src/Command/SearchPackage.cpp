#include "Command.hpp"
#include <curl/curl.h>
#include <curl/easy.h>
#include <nlohmann/json.hpp>
#include "../Utils/General.hpp"
#include "termcolor/termcolor.hpp"
#include <termcolor/termcolor.hpp>



namespace Command {
  using nlohmann::json;

  int levensteinDistance(std::string aStr, std::string bStr){
    //minimize the amount of memory used
    std::string* a = &aStr;
    std::string* b = &bStr;
    if(aStr.length() > bStr.length()){
      a = &bStr;
      b = &aStr;
    }
    int aLen = a->length();
    int bLen = b->length();
    int* prev = new int[aLen + 1];
    int* curr = new int[aLen + 1];
    for(int i = 0; i <= aLen; i++){
      prev[i] = i;
    }

    for(int i = 1; i <= bLen; i++){
      curr[0] = i;
      for(int j = 1; j <= aLen; j++){
        int cost = (*a)[j - 1] == (*b)[i - 1] ? 0 : 1;
        curr[j] = std::min(std::min(curr[j - 1] + 1, prev[j] + 1), prev[j - 1] + cost);
      }
      int* temp = prev;
      prev = curr;
      curr = temp;
    }
    int result = prev[aLen];
    delete[] prev;
    delete[] curr;
    return result;
  }
  int getStringScore(std::string &text, std::string &query){
    int score = 0;

    if(text.size() == 0 || query.size() == 0){
      return score;
    }
    if(text == query){
      score += 100;
      return score;
    }
    if(levensteinDistance(text, query) < 3){
      score += 30;
    }
    return score;
  }
  std::vector<packageResult> calculatePackageScores(std::string query){
    std::vector<packageResult> results;
    json rawIndex = fetchIndex();
    Utils::toLower(query);
    std::string searchString = "";
    for(json json: rawIndex){
      packageResult package{
          .name = json["name"],
          .url = json["git"],
          .versions = json["versions"],
          .target_link = json.contains("target_link") 
          ? json["target_link"]
          : json["name"],
          .description = json.contains("description")
          ? json["description"]
          : "",
          .score = 0
      };
      searchString = 
        package.name + " " 
        + package.description + " " 
        + package.target_link;
      Utils::toLower(searchString);

      std::cout << "Search String: " << searchString << std::endl;

      std::vector<std::string> searchWords = Utils::split(searchString, ' ');
      for(std::string word: searchWords){
        if(word == ""){
          continue;
        }
        package.score += getStringScore(word, query);
      }

      results.push_back(package);
    }

    std::sort(results.begin(), results.end(), [](packageResult a, packageResult b){
        return a.score > b.score;
        });
    return results;
  }

  std::vector<packageResult> searchPackage(std::string query){
    std::cout << "Searching for package" << std::endl;


    std::vector<packageResult> results = calculatePackageScores(query);


    std::cout << "Results: " << results.size() << std::endl;
    for(int i = results.size();i > -1; i--){
      if(results[i].score > 10){
        //Adjusting the indexes for the normies
        std::cout << "[" << termcolor::green << i << termcolor::white << "]" << "[s:" << results[i].score << "] " << results[i].name << " - (" << termcolor::bright_blue << results[i].url << termcolor::white <<  ")" << std::endl;
        std::cout << "    " << termcolor::cyan << results[i].description << termcolor::white << std::endl;
      }
    }

    return results;
  }

}
