#pragma once
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <termcolor/termcolor.hpp>
#include <vector>
#define DEBUGTHIS(x)                                                           \
  std::cout << termcolor::bright_red << "##x"                                  \
            << " -> " << x << std::endl;

#ifdef DEBUG
#include <cpptrace/cpptrace.hpp>
#endif

#ifdef TEST
#include <cpptrace/cpptrace.hpp>
#endif

namespace Frate::Utils {

  using nlohmann::json;
  /*
   * Gets the folder name which is the current directory name
   * like if you are in /home/user/Downloads it will return Downloads
   * @return The folder name
   */
  std::string getFolderName();

  /*
   * Generates uuid looking like
   * 123e4567-e89b-12d3-a456-426614174000
   * @return The uuid generated
   */
  std::string genUUIDv4();

  using std::filesystem::path;

  /*
   * Generates a random path in the /tmp directory using uuidv4
   * @return The path generated
   */
  path randomTmpPath(std::string prefix = "frate-");

  /*
   * Copies the file to a random path in the /tmp directory
   * Intended to be used for non destructive deletion
   * @param p The path to be copied
   * @return The path of the file in the /tmp directory
   */
  path copyToTmpPath(path p, std::string prefix = "frate-");

  /*
   * Split the string by the delimiter
   * @param str The string to be splitted
   * @param delimiter The delimiter to be splitted by
   * @return The vector of splitted string
   */
  std::vector<std::string> split(std::string str, char delimiter);

  /*
   * Convert the string to lower case
   * @param str The string to be converted
   */
  void toLower(std::string &str);

  /*
   * Fetch the json from the url
   * @param url The url to be fetched
   * @return The json fetched from the url
   */
  json fetchJson(std::string url);

  /*
   * Fetch the text from the url
   * @param url The url to be fetched
   * @return The text fetched from the url
   */
  std::string fetchText(std::string url, bool verbose = false);

  /*
   * Fetches zip, tar, tar.gz, tar.xz, tar.bz2 archives
   * @param url The url to be fetched
   * @param unzip_to The path to unzip to
   */
  void fetchGitArchive(const std::string &git_url, const std::string &branch,
                       const path &unzip_to);
  /*
   * Fetches git projects
   * @param url The url to be fetched
   * @param unzip_to The path to unzip to
   */
  void fetchGitProject(const std::string &git_url, const std::string &branch,
                       const path &clone_to, bool recurse_submodules = false);

  /*
   * Unzips the tar file
   * @param tar_path The path to the tar file
   * @param unzip_to The path to unzip to
   */
  void unzipTar(const path &tar_path, const path &unzip_to);

  /*
   * Unzips the tar.gz file
   * @param tar_path The path to the tar.gz file
   * @param unzip_to The path to unzip to
   */
  void unzipTarGz(const path &tar_path, const path &unzip_to);

  /*
   * Unzips the tar.xz file
   * @param tar_path The path to the tar.xz file
   * @param unzip_to The path to unzip to
   */
  void unzipTarXz(const path &tar_path, const path &unzip_to);

  /*
   * Unzips the tar.bz2 file
   * @param tar_path The path to the tar.bz2 file
   * @param unzip_to The path to unzip to
   */
  void unzipTarBz2(const path &tar_path, const path &unzip_to);

  /*
   * Unzips the zip file
   * @param zip_path The path to the zip file
   * @param unzip_to The path to unzip to
   */
  void unzipZip(const path &zip_path, const path &unzip_to,
                std::string internal_path = "");

  /*
   * A new Utils::hSystem command which returns the exit code of the command
   * And we also take std::string
   * @param command The command to be executed
   * @return The exit code of the command
   */
  int hSystem(std::string command);

  using CmdOutput = struct CmdOutputS {
    std::string std_out;
    std::string std_err;
    int return_code;
  };

  /*
   * Runs the command and returns all the output of the command using a
   * CmdOutput struct
   * @param cmd The command to be executed
   * @return The output of the command
   */
  CmdOutput hSystemWithOutput(std::string cmd);

  struct TableFormat {
    int width{};
    char fill{};
    TableFormat() = default;

    template <typename T> TableFormat &operator<<(const T &data) {
      std::cout << std::setw(width) << data << std::setfill(fill);
      return *this;
    }
  };

  /*
   * F*CKING MAGIC
   */
  int levensteinDistance(std::string a_str, std::string b_str);

  /*
   * This is a function get a score between two strings, essentialy how similar
   * they are
   * @param text The text to be compared
   * @param query The query to be compared
   * @return The score of the two strings
   */
  int getStringScore(std::string &text, std::string &query);

  /*
   * Debug stuff
   * Prints out a stack trace
   */
  void debug(std::string something);

  /*
   * replaces the provided key with the value
   */
  void replaceKey(std::string &str, std::string key, std::string value);
} // namespace Frate::Utils
