#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <iomanip>
#include <iostream>
#include <ldap.h>
#include <termcolor/termcolor.hpp>
#define DEBUGTHIS(x) std::cout << termcolor::bright_red << "##x" << " -> " << x << std::endl;

#ifdef DEBUG
#include <cpptrace/cpptrace.hpp>
#endif

#ifdef TEST 
#include <cpptrace/cpptrace.hpp>
#endif

namespace Frate {
  static bool verbose_mode = false;
}

namespace Frate::Utils {
  class Error {
    public:
      template<typename T>
      Error& operator<<(const T& data) {
        std::cout << termcolor::red << data << termcolor::reset;
        return *this;
      }
      Error& operator<<(std::ostream& (*pf)(std::ostream&)) {
        std::cout << pf;
        return *this;
      }
  };
  class Warning {
    public:
      template<typename T>
      Warning& operator<<(const T& data) {
        std::cout << termcolor::yellow << data << termcolor::reset;
        return *this;
      }
      Warning& operator<<(std::ostream& (*pf)(std::ostream&)) {
        std::cout << pf;
        return *this;
      }
  };
  class Info {
    public:
      template<typename T>
      Info& operator<<(const T& data) {
        std::cout << termcolor::green << data << termcolor::reset;
        return *this;
      }
      Info& operator<<(std::ostream& (*pf)(std::ostream&)) {
        std::cout << pf;
        return *this;
      }
  };
  class Verbose {
    public:
      template<typename T>
      Verbose& operator<<(const T& data) {
        if(Frate::verbose_mode){
          std::cout << termcolor::bright_blue << data << termcolor::reset;
        }
        return *this;
      }
      Verbose& operator<<(std::ostream& (*pf)(std::ostream&)) {
        if(Frate::verbose_mode){
          std::cout << pf;
        }
        return *this;
      }
  };
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
