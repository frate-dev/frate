#include <Frate/System/GitCommit.hpp>
#include <ostream>

namespace Frate::System {
  GitCommit::GitCommit() = default;

  std::ostream &operator<<(std::ostream &os_stream, const GitCommit &commit) {
    os_stream << "Hash: " << commit.hash << std::endl;
    os_stream << "Author: " << commit.author << std::endl;
    os_stream << "Date: " << commit.date << std::endl;
    os_stream << "Message: " << commit.message << std::endl;
    return os_stream;
  }
} // namespace Frate::System
