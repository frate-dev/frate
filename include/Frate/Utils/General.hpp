#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <iomanip>
#include <iostream>
#include <termcolor/termcolor.hpp>
#define DEBUGTHIS(x) std::cout << termcolor::bright_red << "##x" << " -> " << x << std::endl;

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
  path copyToTmpPath(path p,std::string prefix = "frate-");

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
  void toLower(std::string& str);

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
  std::string fetchText(std::string url,bool verbose = false);

  /*
   * A new Utils::hSystem command which returns the exit code of the command
   * And we also take std::string
   * @param command The command to be executed
   * @return The exit code of the command
   */
  int hSystem(std::string command);


  typedef struct CmdOutput_s {
    std::string std_out;
    std::string std_err;
    int return_code;
  } CmdOutput;

  /*
   * Runs the command and returns all the output of the command using a CmdOutput struct
   * @param cmd The command to be executed
   * @return The output of the command
   */
  CmdOutput hSystemWithOutput(std::string cmd);
  struct TableFormat {
      int width;
      char fill;
      TableFormat(): width(14), fill(' ') {}
      template<typename T>
      TableFormat& operator<<(const T& data) {
          std::cout<<  std::setw(width) << data << std::setfill(fill);
          return *this;
      }
  };
  /*
   * F*CKING MAGIC
   */
  int levensteinDistance(std::string aStr, std::string bStr);
  int getStringScore(std::string &text, std::string &query);


  /*
   * Debug stuff
   */
  void debug(std::string something);


  /*
   * replaces the provided key with the value
   */
  void replaceKey(std::string &str, std::string key, std::string value);
}
