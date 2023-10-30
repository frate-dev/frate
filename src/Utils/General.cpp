#include <filesystem>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
namespace Utils{

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
        std::cout << "Failed to request " << url << std::endl;
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
    if(result.size() == 0){
      result.push_back(word);
    }
    return result;
  }
  //"https://github.com/cmaker-dev/index/releases/latest/download/index.json"
  std::string fetchText(std::string url) {
    std::cout << "Downloading index.json" << std::endl;
    std::string requrl = url;
    CurlResponse r = HttpGet(requrl);
    std::cout << r.status_code << std::endl;
    
    switch(r.status_code){
      case 200:
        std::cout << "Downloaded index.json" << std::endl;
        break;
      case 404:
        std::cout << "Failed to download index.json" << std::endl;
        std::cout << "Error: " << r.error << std::endl;
        exit(-1);
        break;
      default:
        std::cout << "Failed to download index.json" << std::endl;
        std::cout << "Error: " << r.error << std::endl;
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
      std::cout << "Failed to parse index.json" << std::endl;
      std::cout << "Error: " << e.what() << std::endl;
      std::cout << "At: " << e.byte << std::endl;
      std::cout << "Text: " << responseStr << std::endl;
      exit(-1);
    }
  }
}
