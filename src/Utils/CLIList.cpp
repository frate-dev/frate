#include "CLI.hpp"
#include <sstream>
#include <string>
#include <stdint.h>
#include <iostream>
#include <termcolor/termcolor.hpp>



namespace Utils::CLI {
  ListItem::ListItem(std::string primary, std::string subtext){
    this->primary = primary;
    this->subtext = subtext;
  }
  List::List(){
    numbered = false;
    stream = std::stringstream();
    index_color   = Ansi::GREEN;
    primary_color = Ansi::WHITE;
    subtext_color = Ansi::BLUE;
  }
  List::List(std::string _title){
    title = _title;
    List();
  }
  List::~List(){
  }
  List* List::IndexColor(std::string color){
    index_color = color;
    return this;
  }
  List* List::PrimaryColor(std::string color){
    primary_color = color;
    return this;
  }
  List* List::SubTextColor(std::string color){
    subtext_color = color;
    return this;
  }
  List* List::Numbered(){
    numbered = true;
    return this;
  }
  List* List::ReverseIndexed(){
    reversed_index = true;
    return this;
  }
  void List::push_line(int index, ListItem& item){
    if(numbered){
      stream << index_color << "[" << index << "] " << primary_color << item.primary;
      if(item.subtext != ""){
        stream << "\n\t" << subtext_color << item.subtext;
      }
      stream << "\n";
    }else{
      stream << primary_color << item.primary;
      if(item.subtext != ""){
        stream << "\n\t" << subtext_color << item.subtext;
      }
      stream << "\n";
    }
  }
  std::string List::Build(){
    if(reversed_index){
      for(int i = items.size() - 1; i >= 0; i--){
        push_line(i, items[i]);
      }
    }else{
      for(size_t i = 0; i < items.size(); i++){
        push_line(i, items[i]);
      }
    }
    stream << Ansi::RESET;
    return stream.str();
  }
  void List::pushFront(ListItem item){
    items.insert(items.begin(), item);
  }
  void List::pushBack(ListItem item){
    items.push_back(item);
  }
}
