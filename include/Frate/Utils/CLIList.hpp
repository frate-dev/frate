#pragma once
#include <string>
#include <vector>
#include <sstream>
namespace Frate::Utils::CLI {

  typedef struct ListItem {
    std::string primary;
    std::string subtext;
    ListItem(std::string primary, std::string subtext = "");
  } ListItem;

  class List {
  private:
    std::string index_color;
    std::string primary_color;
    std::string subtext_color;
    std::string title;
    bool reversed_index{false};
    bool numbered{false};
    std::vector<ListItem> items;
    void push_line(int index, ListItem &item);

  public:
    std::stringstream stream;

    List();
    List(std::string title);
    ~List()= default;
    /*
     * Sets the color of the index
     * [index] <--- primary
     *    [subtext]
     */
    List &IndexColor(std::string);
    /*
     * Sets the primary color
     * [index] primary <----
     *    [subtext]
     */
    List &PrimaryColor(std::string);
    /*
     * Sets the subtext color
     * [index] primary
     *   [subtext] <----
     */
    List &SubTextColor(std::string);
    /*
     * If the list should be numbered
     */
    List &Numbered();
    /*
     * Displays the index in reverse order
     */
    List &ReverseIndexed();
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
