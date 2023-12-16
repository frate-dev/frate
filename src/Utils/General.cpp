#include "Frate/Utils/General.hpp"
#include <Frate/Generators.hpp>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <curl/curl.h>

namespace Frate::Utils{

  struct CurlResponse {
    std::string text;
    int status_code;
    std::string error;
  };

  // Callback function to handle incoming data
  size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
  }

  CurlResponse HttpGet(std::string& url) {
    CurlResponse response;
CURL* curl = curl_easy_init();
    if (!curl) {
        response.error = "Failed to initialize libcurl.";
        return response;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // Set the callback function to handle the response body
    std::string responseBody;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    //https options
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        Utils::error << "Failed to request " << url << std::endl;
        response.error = curl_easy_strerror(res);
        curl_easy_cleanup(curl);
        return response;
    }

    // Get the response code
    long responseCode;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);

    response.text = responseBody;
    response.status_code = static_cast<int>(responseCode);

    curl_easy_cleanup(curl);
    return response;
  }

  using nlohmann::json;
  std::string getFolderName(){
    std::string directory_name = std::filesystem::current_path();
    directory_name = directory_name.substr(directory_name.find_last_of("/\\") + 1);
    return directory_name;
  }

  std::vector<std::string> split(std::string str, char delimiter){
    std::vector<std::string> result;

    std::string word = "";

    for (char x : str) {
      if (x == delimiter) {
        result.push_back(word);
        word = "";
      } else {
        word = word + x;
      }
    }
    if(word.size() > 0){
      result.push_back(word);
    }
    return result;
  }
  void toLower(std::string &str){
    for (size_t i = 0; i < str.size(); i++) {
      str[i] = tolower(str[i]);
    }
  }
  //"https://github.com/frate-dev/index/releases/latest/download/index.json"
  std::string fetchText(std::string url,bool verbose) {
    std::string requrl = url;
    CurlResponse r = HttpGet(requrl);
    if(verbose) Utils::info << "Attempting to download: " << termcolor::bright_blue << url << std::endl;
    
    switch(r.status_code){
      case 200:
        if(verbose) Utils::info << "Successfully downloaded: " << url << std::endl;
        break;
      case 404:
        Utils::error << "Failed to download: " << url <<std::endl;
        Utils::error << "Error: " << r.error << std::endl;
        exit(-1);
        break;
      default:
        Utils::error << "Failed to download: " << url << std::endl;
        Utils::error << "Error: " << r.error << std::endl;
        exit(-1);
        break;
    }
    return r.text;
  }
  json fetchJson(std::string url) {
    std::string responseStr = fetchText(url);
    try {
      return json::parse(responseStr);
    } catch (json::parse_error& e) {
      Utils::error << "At: " << e.byte << std::endl;
      Utils::error << "Error: " << e.what() << std::endl;
      Utils::error << "Failed to parse index.json" << std::endl;
      Utils::error << "Text: " << responseStr << std::endl;
      Utils::debug("Failed to parse index.json");
      exit(-1);
    }
  }
  int hSystem(std::string cmd){
    int return_code = std::system(cmd.c_str());
    if(WIFEXITED(return_code)){
      return WEXITSTATUS(return_code);
    }else{
      return -1;
    }
  }

  std::string genUUIDv4(){
    std::string uuid = "";
    //Pattern: xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx
    std::string chars = "0123456789abcdef";
    //Seeding the random number gererator with time
    std::srand(std::time(nullptr));

    for(int i = 0; i < 8; i++){
      uuid += chars[std::rand() % chars.length()];
    }
    uuid += "-";
    for(int i = 0; i < 4; i++){
      uuid += chars[std::rand() % chars.length()];
    }
    uuid += "-4";
    for(int i = 0; i < 3; i++){
      uuid += chars[std::rand() % chars.length()];
    }
    uuid += "-";
    uuid += chars[std::rand() % 4 + 8];
    for(int i = 0; i < 3; i++){
      uuid += chars[std::rand() % chars.length()];
    }
    uuid += "-";
    for(int i = 0; i < 12; i++){
      uuid += chars[std::rand() % chars.length()];
    }
    return uuid;
  }

  using std::filesystem::path;
  path randomTmpPath(std::string prefix){
    path tmp_path = std::filesystem::temp_directory_path();
    tmp_path /= prefix + genUUIDv4();
    return tmp_path;
  }

  path copyToTmpPath(path p,std::string prefix){
    path tmp_path = randomTmpPath(prefix);
    Utils::info << "Copying " << p << " to " << tmp_path << std::endl;
    if(!std::filesystem::exists(p)){
      Utils::error << "Failed to copy " << p << " to " << tmp_path 
        << " when attempting to non destructively delete" << std::endl;
      return tmp_path;
    }
    try{
      std::filesystem::copy(p, tmp_path, std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
    }catch(std::filesystem::filesystem_error& e){
      Utils::error << "Failed to copy " << p << " to " << tmp_path 
        << " when attempting to non destructively delete" << std::endl;

      Utils::error << "Error: " << e.what() << std::endl;
      exit(-1);
    }
    return tmp_path;
  }

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
    int* curr = new int[aLen + 1];
    int* prev = new int[aLen + 1];
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
    for(std::string word: split_text){
      Utils::toLower(word);
      Utils::toLower(query);
      if(word == query){
        score += 100;
      }
      if(word.find(query) != std::string::npos){
        score += 50;
      }
      if(std::abs((int)text.size() - (int)query.size()) < 3){
        score += 1;
      }
    }
    if(Utils::levensteinDistance(text, query) < 3){
      score += 30;
    }
    return score;
  }
  void replaceKey(std::string &str, std::string key, std::string value){
    std::string key_start = key;
    while(str.find(key_start) != std::string::npos){
      str.replace(str.find(key_start), key_start.length(), value);
    }
  }
}
