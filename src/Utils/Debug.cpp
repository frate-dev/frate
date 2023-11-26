#include <stdint.h>
#include <termcolor/termcolor.hpp>
#include <Frate/Utils/General.hpp>


namespace Utils {
  void debug(std::string something){
#ifdef DEBUG
    cpptrace::generate_trace().print();
    DEBUGTHIS(something);
#elif TEST
    cpptrace::generate_trace().print();
    DEBUGTHIS(something);
#else
    std::cout << termcolor::bright_red << something << std::endl;
#endif
  }
}
