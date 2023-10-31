#include "Command.hpp"
#include <curl/curl.h>
#include <curl/easy.h>
#include <nlohmann/json.hpp>
#include "../Utils/General.hpp"



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
  std::vector<packageResult> calculatePackageScores(std::vector<std::string> queryTokens){
    std::vector<packageResult> results;
    json rawIndex = Utils::fetchJson("https://github.com/cmaker-dev/index/releases/latest/download/index.json"); 
    int lengthDiffAbs = 0;
    for(std::string query: queryTokens){
      for(json package: rawIndex){
        //TODO: add description that can be searched
        results.push_back({
            .name = package["name"],
            .url = package["git"],
            .versions = package["versions"],
            .target_link = package.contains("target_link") 
              ? package["target_link"]
              : package["name"],
            .score = 0
            });
        //if it is an exact match
        if(results.back().name == query){
          results.back().score += 100;
        }
        //Check if the query is in the package name
        if(results.back().name.find(query) != std::string::npos){
          results.back().score += 50;
        }
        if(levensteinDistance(results.back().name, query) < 3){
          results.back().score += 10;
        }

        lengthDiffAbs = std::abs((int)results.back().name.length() - (int)query.length());
        if(results.back().score > 20){
          results.back().score -= lengthDiffAbs;
        }
      }
    }
    std::sort(results.begin(), results.end(), [](packageResult a, packageResult b){
        return a.score > b.score;
        });
    std::vector<packageResult> resultsUnique;

    for(packageResult result: results){
      if(std::any_of(resultsUnique.begin(), resultsUnique.end(), [&result](packageResult &r){
            r.score += result.score;
            return r.name == result.name;
            })){
        continue;
      }
      resultsUnique.push_back(result);
    }
    return resultsUnique;
  }

  std::vector<packageResult> searchPackage(std::string query){
    std::cout << "Searching for package" << std::endl;

    std::vector<std::string> queryTokens = Utils::split(query, ' ');

    std::vector<packageResult> results = calculatePackageScores(queryTokens);


    std::cout << "Results:" << std::endl;
    for(int i = results.size();i > -1; i--){
      if(results[i].score > 10){
        //Adjusting the indexes for the normies
        std::cout << "[" << i << "]" << "[s:" << results[i].score << "] " << results[i].name << " - (" << results[i].url << ")" << std::endl;
      }
    }

    return results;
  }

}
