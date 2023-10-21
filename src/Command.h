#include <memory> 
#include <string>

namespace Command {

  bool handleCppProject();

  bool handleCProject();

   
  void loadPackageToml();


  typedef struct Context {
     std::string name;
  } Context;

  int init(std::shared_ptr<Context>);
  int addLib(std::shared_ptr<Context>, std::string);
};
