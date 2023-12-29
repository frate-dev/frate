#include <Frate/System/GitRef.hpp>

namespace Frate::System {
  GitRef::GitRef() = default;

  std::ostream &operator<<(std::ostream &os_stream, const GitRef &ref) {
    os_stream << (ref.name + " " + ref.hash);
    return os_stream;
  }

} // namespace Frate::System
