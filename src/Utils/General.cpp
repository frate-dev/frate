#include "Frate/Utils/General.hpp"
#include "Frate/Utils/Logging.hpp"
#include <Frate/Generators.hpp>
#include <curl/curl.h>
#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

namespace Frate::Utils {

  const int HTTP_STATUS_OK = 200;
  const int HTTP_STATUS_NOT_FOUND = 404;
  const int HTTP_STATUS_SERVER_ERROR = 500;

  struct CurlResponse {
    std::string text;
    int status_code{};
    std::string error;
  };

  // Callback function to handle incoming data
  size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                       std::string *output) {
    size_t total_size = size * nmemb;
    output->append(static_cast<char *>(contents), total_size);
    return total_size;
  }

  CurlResponse HttpGet(std::string &url) {
    CurlResponse response;
    CURL *curl = curl_easy_init();
    if (curl == nullptr) {
      response.error = "Failed to initialize libcurl.";
      return response;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // Set the callback function to handle the response body
    std::string responseBody;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    // https options
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
    long response_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    response.text = responseBody;
    response.status_code = static_cast<int>(response_code);

    curl_easy_cleanup(curl);
    return response;
  }

  using nlohmann::json;

  std::string getFolderName() {
    std::string directory_name = std::filesystem::current_path();
    directory_name =
        directory_name.substr(directory_name.find_last_of("/\\") + 1);
    return directory_name;
  }

  std::vector<std::string> split(std::string str, char delimiter) {
    std::vector<std::string> result;

    std::string word;

    for (char curr_char : str) {
      if (curr_char == delimiter) {
        result.emplace_back(word);
        word = "";
      }
      else {
        word = word + curr_char;
      }
    }
    if (!word.empty()) {
      result.emplace_back(word);
    }
    return result;
  }

  void toLower(std::string &str) {
    for (char &curr_char : str) {
      curr_char = static_cast<char>(tolower(curr_char));
    }
  }

  //"https://github.com/frate-dev/index/releases/latest/download/index.json"
  std::string fetchText(std::string url, bool verbose) {
    std::string requrl = url;
    CurlResponse response = HttpGet(requrl);
    if (verbose) {
      Utils::info << "Attempting to download: " << termcolor::bright_blue << url
                  << std::endl;
    }

    switch (response.status_code) {
    case HTTP_STATUS_OK:
      if (verbose)
        Utils::info << "Successfully downloaded: " << url << std::endl;
      break;
    case HTTP_STATUS_NOT_FOUND:
      Utils::error << "Failed to download: " << url << std::endl;
      Utils::error << "Error: " << response.error << std::endl;
      exit(-1);
      break;
    default:
      Utils::error << "Failed to download: " << url << std::endl;
      Utils::error << "Error: " << response.error << std::endl;
      exit(-1);
      break;
    }
    return response.text;
  }

  json fetchJson(std::string url) {
    std::string response_str = fetchText(url);
    try {
      return json::parse(response_str);
    } catch (json::parse_error &e) {
      Utils::error << "At: " << e.byte << std::endl;
      Utils::error << "Error: " << e.what() << std::endl;
      Utils::error << "Failed to parse index.json" << std::endl;
      Utils::error << "Text: " << response_str << std::endl;
      Utils::debug("Failed to parse index.json");
      throw e;
    }
  }

  /*
   * If we have a internal path we're going to take the extracted zip and
   * move the contents of that interal path to the unzip_to path for
   * example if we have a zip file with the following structure
   * test.zip \
   *          test \
   *            test.txt
   *            test2.txt
   *            test3.txt
   *            test4.txt
   * and we want to extract the test folder to the current directory we
   * would do the following
   * unzipZip("test.zip", ".", "test");
   * and the result in the unzip_to directory would be
   * unzip_dir \
   *  test.txt
   *  test2.txt
   *  test3.txt
   *  test4.txt
   */
  void unzipZip(const path &zip_path, const path &unzip_to,
                std::string internal_path) {

    std::filesystem::path zip_dir = zip_path.parent_path();

    Utils::info << "Unzipping: " << zip_path << std::endl;
    std::string cmd =
        "unzip -q " + zip_path.string() + " -d " + zip_dir.string();

    int return_code = Utils::hSystem(cmd);
    if (return_code != 0) {
      Utils::error << "Failed to unzip: " << zip_path << std::endl;
      exit(-1);
    }

    if (!internal_path.empty()) {
      std::filesystem::create_directories(unzip_to);
      for (std::filesystem::directory_entry current_path :
           std::filesystem::recursive_directory_iterator(zip_dir /
                                                         internal_path)) {

        std::filesystem::path current_path_relative =
            std::filesystem::relative(current_path, zip_dir / internal_path);
        std::filesystem::path new_path = unzip_to / current_path_relative;

        if (std::filesystem::is_directory(current_path)) {
          Utils::verbose << "Creating directory: " << new_path << std::endl;

          std::filesystem::create_directories(new_path);
        }
        else {
          Utils::verbose << "Copying: " << current_path << " to " << new_path
                         << std::endl;

          std::filesystem::copy(current_path, new_path);
        }
      }
    }
  }

  void fetchGitArchive(const std::string &git_url, const std::string &branch,
                       const std::filesystem::path &unzip_to) {

    std::string stripped_git_url = git_url;
    // Removes .git from the end of the url
    if (stripped_git_url.find(".git") != std::string::npos) {
      Utils::replaceKey(stripped_git_url, ".git", "");
    }
    /*
     * Uses the stripped github url to get the archive url to generate a url
     * like https://www.github.com/frate-dev/index/archive/refs/heads/main.zip
     */
    std::string url =
        stripped_git_url + "/archive/refs/heads/" + branch + ".zip";
    /*
     * Gets the last part of the stripped github url so that it goes from
     * https://www.github.com/frate-dev/index to index
     */
    std::string repo_name =
        stripped_git_url.substr(git_url.find_last_of("/\\") + 1);

    Utils::info << "Downloading archive: " << url << std::endl;
    std::string requrl = url;
    CurlResponse res = HttpGet(requrl);

    std::filesystem::path tmp_path = Utils::randomTmpPath("frate-git-archive-");

    switch (res.status_code) {
    case HTTP_STATUS_OK:
      break;
    case HTTP_STATUS_NOT_FOUND:
      Utils::error << "Failed to download: " << url << std::endl;
      Utils::error << "Error: " << res.error << std::endl;
      exit(-1);
      break;
    default:
      Utils::error << "Failed to download: " << url << std::endl;
      Utils::error << "Error: " << res.error << std::endl;
      exit(-1);
      break;
    }
    // gets the last part of the url
    std::string filename = url.substr(url.find_last_of("/\\") + 1);
    std::filesystem::path archive_path = tmp_path / filename;
    std::ofstream file(archive_path, std::ios::binary);

    file << res.text;
    file.close();
    Utils::unzipZip(archive_path, unzip_to, repo_name + "-" + branch);

    std::filesystem::path sub_directory_to_extract =
        unzip_to / (repo_name + "-" +
                    branch); // this is the directory that github generates
                             // inside the archive that we would like to
                             // extract to the current folder

    // // Removes the archive
    // std::filesystem::remove_all(tmp_path);

    file << res.text;
    file.close();
  }

  int hSystem(std::string cmd) {
    int return_code = std::system(cmd.c_str());
    if (WIFEXITED(return_code)) {
      return WEXITSTATUS(return_code);
    }
    else {
      return -1;
    }
  }

  CmdOutput hSystemWithOutput(std::string cmd) {
    CmdOutput output;
    std::string cmd_stdout;
    std::string cmd_stderr;
    FILE *pipe = popen(cmd.c_str(), "r");
    if (pipe == nullptr) {
      Utils::error << "Failed to execute command: " << cmd << std::endl;
      exit(-1);
    }
    const int buffer_size = 128;
    char buffer[buffer_size];
    while (!feof(pipe)) {
      if (fgets(buffer, buffer_size, pipe) != nullptr) {
        cmd_stdout += buffer;
      }
    }
    output.std_out = cmd_stdout;
    output.std_err = cmd_stderr;
    output.return_code = pclose(pipe);
    return output;
  }

  std::string genUUIDv4() {
    std::string uuid;
    // Pattern: xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx
    const std::string chars = "0123456789abcdef";
    // Seeding the random number gererator with time
    std::srand(std::time(nullptr));
    for (int i = 0; i < 8; i++) {
      uuid += chars[std::rand() % chars.length()];
    }
    uuid += "-";
    for (int i = 0; i < 4; i++) {
      uuid += chars[std::rand() % chars.length()];
    }
    uuid += "-4";
    for (int i = 0; i < 3; i++) {
      uuid += chars[std::rand() % chars.length()];
    }
    uuid += "-";
    uuid += chars[std::rand() % 4 + 8];
    for (int i = 0; i < 3; i++) {
      uuid += chars[std::rand() % chars.length()];
    }
    uuid += "-";
    for (int i = 0; i < 12; i++) {
      uuid += chars[std::rand() % chars.length()];
    }
    return uuid;
  }

  using std::filesystem::path;

  path randomTmpPath(std::string prefix) {
    path tmp_path = std::filesystem::temp_directory_path();
    tmp_path /= prefix + genUUIDv4();
    std::filesystem::create_directories(tmp_path);
    return tmp_path;
  }

  path copyToTmpPath(path path_to_copy, std::string prefix) {
    path tmp_path = randomTmpPath(prefix);
    Utils::info << "Copying " << path_to_copy << " to " << tmp_path
                << std::endl;
    if (!std::filesystem::exists(path_to_copy)) {
      Utils::error << "Failed to copy " << path_to_copy << " to " << tmp_path
                   << " when attempting to non destructively delete"
                   << std::endl;
      return tmp_path;
    }
    try {
      std::filesystem::copy(
          path_to_copy, tmp_path,
          std::filesystem::copy_options::recursive |
              std::filesystem::copy_options::overwrite_existing);
    } catch (std::filesystem::filesystem_error &e) {
      Utils::error << "Failed to copy " << path_to_copy << " to " << tmp_path
                   << " when attempting to non destructively delete"
                   << std::endl;

      Utils::error << "Error: " << e.what() << std::endl;
      exit(-1);
    }
    return tmp_path;
  }

  /*
   * FUCKING MAGIC
   */
  int levensteinDistance(std::string a_str, std::string b_str) {
    std::string *a = &a_str;
    std::string *b = &b_str;
    if (a_str.length() > b_str.length()) {
      a = &b_str;
      b = &a_str;
    }
    int aLen = a->length();
    int bLen = b->length();
    int *curr = new int[aLen + 1];
    int *prev = new int[aLen + 1];
    for (int i = 0; i <= aLen; i++) {
      prev[i] = i;
    }

    for (int i = 1; i <= bLen; i++) {
      curr[0] = i;
      for (int j = 1; j <= aLen; j++) {
        int cost = (*a)[j - 1] == (*b)[i - 1] ? 0 : 1;
        curr[j] = std::min(std::min(curr[j - 1] + 1, prev[j] + 1),
                           prev[j - 1] + cost);
      }
      int *temp = prev;
      prev = curr;
      curr = temp;
    }
    int result = prev[aLen];
    delete[] prev;
    delete[] curr;
    return result;
  }

  int getStringScore(std::string &text, std::string &query) {
    int score = 0;
    std::vector<std::string> split_text = Utils::split(text, ' ');
    for (std::string word : split_text) {
      Utils::toLower(word);
      Utils::toLower(query);
      if (word == query) {
        score += 100;
      }
      if (word.find(query) != std::string::npos) {
        score += 50;
      }
      if (std::abs((int)text.size() - (int)query.size()) < 3) {
        score += 1;
      }
    }
    if (Utils::levensteinDistance(text, query) < 3) {
      score += 30;
    }
    return score;
  }

  void replaceKey(std::string &str, std::string key, std::string value) {
    std::string key_start = key;
    while (str.find(key_start) != std::string::npos) {
      str.replace(str.find(key_start), key_start.length(), value);
    }
  }
} // namespace Frate::Utils
