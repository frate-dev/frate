#include "Command.hpp"
#include <curl/curl.h>
#include <curl/easy.h>
#include <nlohmann/json.hpp>



namespace Command {
//   const std::string RepoIndex = "https://raw.githubusercontent.com/cmaker-dev/index/main/index.json";
//   void writeCallback(char* ptr, size_t size, size_t nmemb, std::string* data) {
//     data->append(ptr, size * nmemb);
//   }
//   void SearchPackage(){
//     curl_easy_init();
//     std::shared_ptr<CURL> curl(curl_easy_init(), curl_easy_cleanup);
// 
//     curl_easy_setopt(curl.get(), CURLOPT_URL, RepoIndex.c_str());
// 
//     std::string response;
// 
//     curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, writeCallback);
//     curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &response);
// 
//     curl_easy_perform(curl.get());
// 
//     nlohmann::json j = nlohmann::json::parse(response);
// 
//     std::cout << j.dump(4) << std::endl;
// 
// 
// 
//   }


}
