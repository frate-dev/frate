#include <memory> 
#include <string>

namespace Command {
  typedef struct Context {
     std::string name;
  } Context;

  int init(std::shared_ptr<Context>);
  int addLib(std::shared_ptr<Context>, std::string);
};
