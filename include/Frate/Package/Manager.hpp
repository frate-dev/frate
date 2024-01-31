#include <string>
#include <vector>
#include <Frate/Package/IndexEntry.hpp>

namespace Frate::Package {
  class Manager {
    public:
      void add(std::string name, std::string version);
      void remove(std::string name);
      void list();
      IndexEntry get(std::string name);
      std::vector<IndexEntry> getAll();
  };
}
