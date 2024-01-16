#include <Frate/Utils/General.hpp>
#include <Frate/Utils/Logging.hpp>

namespace Frate::Utils {
  void debug(std::string something) {
#ifdef DEBUG
    cpptrace::generate_trace().print();
    Utils::error << something << std::endl;
#elif TEST
    cpptrace::generate_trace().print();
    Utils::error << something << std::endl;
#else
    std::cout << termcolor::bright_red << something << std::endl;
#endif
  }
} // namespace Frate::Utils
