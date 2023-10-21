#include <iostream>


namespace Command{

  int help() {
    std::cout << "usage: "
      << "\n";
    return 1;
  }

}
