#include <Frate/System/GitStatus.hpp>

namespace Frate::System {
  GitStatus::GitStatus() = default;

  std::ostream &operator<<(std::ostream &os_stream, const GitStatus &status) {
    os_stream << std::string("added: ");
    for (std::string file : status.added) {
      os_stream << file + " ";
    }
    os_stream << std::string("\nmodified: ");
    for (std::string file : status.modified) {
      os_stream << file + " ";
    }
    os_stream << std::string("\ndeleted: ");
    for (std::string file : status.deleted) {
      os_stream << file + " ";
    }
    os_stream << std::string("\nrenamed: ");
    for (std::string file : status.renamed) {
      os_stream << file + " ";
    }
    os_stream << std::string("\ncopied: ");
    for (std::string file : status.copied) {
      os_stream << file + " ";
    }
    os_stream << std::string("\nuntracked: ");
    for (std::string file : status.untracked) {
      os_stream << file + " ";
    }
    os_stream << std::string("\nignored: ");
    for (std::string file : status.ignored) {
      os_stream << file + " ";
    }
    return os_stream;
  }
} // namespace Frate::System
