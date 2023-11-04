#include <sstream>
#include <string>
#include <stdint.h>
#include <iostream>
#include <termcolor/termcolor.hpp>
#include <vector>



namespace Utils::CLI {
  //ansicolors
  namespace Ansi{
    const std::string RESET = "\033[0m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string WHITE = "\033[37m";
    const std::string BRIGHT_RED = "\033[91m";
  }
  typedef struct ListItem {
    std::string primary;
    std::string subtext;
    ListItem(std::string primary, std::string subtext);
  } ListItem;
  class List {
    private:
      std::string index_color;
      std::string primary_color;
      std::string subtext_color;
      bool reversed_index;
      bool numbered;
      std::vector<ListItem> items;
      void push_line(int index, ListItem& item);
    public:
      std::stringstream stream;

      List();
      ~List();
      /*
       * Sets the color of the index
       * [index] <--- primary
       *    [subtext]
       */
      List* IndexColor(std::string);
      /*
       * Sets the primary color
       * [index] primary <----
       *    [subtext]
       */
      List* PrimaryColor(std::string);
      /*
       * Sets the subtext color
       * [index] primary
       *   [subtext] <----
       */
      List* SubTextColor(std::string);
      /*
       * If the list should be numbered
       */
      List* Numbered();
      /*
       * Displays the index in reverse order
       */
      List* ReverseIndexed();
      /*
       * Builds the list
       * intdended to be used like this:
       * std::cout << List->Build() << std::endl;
       */
      std::string Build();
      /*
       * Adds a new line to the list
       * @param item the item to be added
       */
      void pushFront(ListItem item);
      /*
       * Adds a new line to the list
       * @param item the item to be added
       */
      void pushBack(ListItem item);
  };
}
