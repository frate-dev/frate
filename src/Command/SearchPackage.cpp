#include "Command.hpp"
#include <nlohmann/json.hpp>
#include "../Utils/General.hpp"
#include "../Utils/CLI.hpp"
#include "termcolor/termcolor.hpp"
#include <termcolor/termcolor.hpp>



namespace Command {
  using nlohmann::json;
  using Utils::CLI::List;
  using Utils::CLI::ListItem;

  /*
   * FUCKING MAGIC
   */
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
    std::vector<std::string> split_text = Utils::split(text, ' ');
    std::vector<std::string> split_query = Utils::split(query, ' ');
    for(std::string word: split_text){
      for(std::string query_word: split_query){
        Utils::toLower(word);
        Utils::toLower(query_word);
        if(word == query_word){
          score += 100;
        }
        if(word.find(query_word) != std::string::npos){
          score += 50;
        }
        if(std::abs((int)text.size() - (int)query.size()) < 3){
          score += 1;
        }
      }
    }
    if(levensteinDistance(text, query) < 3){
      score += 30;
    }
    return score;
  }
  void getPackageScore(Package &package, std::string &query){
    int score = 0;
    score += getStringScore(package.name, query) * 10;
    score += getStringScore(package.description, query);
    score += getStringScore(package.target_link, query);
    package.score = score;
  }
  std::vector<Package> calculatePackageScores(std::string query){
    std::vector<Package> results;
    json rawIndex = fetchIndex();
    Utils::toLower(query);
    for(json json: rawIndex){
      Package package{
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

      getPackageScore(package, query);



      results.push_back(package);
    }

    std::sort(results.begin(), results.end(), [](Package a, Package b){
        return a.score > b.score;
        });
    return results;
  }

  std::vector<Package> searchPackage(std::string query){
    std::cout << "Searching for package" << std::endl;


    std::vector<Package> results = calculatePackageScores(query);


    std::cout << "Results: " << results.size() << std::endl;
    List *list = (new Utils::CLI::List())->
      Numbered()->
      ReverseIndexed();
    for(Package result: results){
      if(result.score > 10){
        list->pushBack(ListItem(result.name + " (" + result.url + ")", result.description));
      }
    }
    std::cout << list->Build() << std::endl;

    return results;
  }

}
