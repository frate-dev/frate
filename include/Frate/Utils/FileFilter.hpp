#include <filesystem>
#include <vector>

namespace Frate::Utils {
  class FileFilter {
  private:
    std::vector<std::string> extensions;
    std::vector<std::string> prefixes;
    std::vector<std::filesystem::path> paths;

    std::filesystem::path root_path;

    bool is_valid(const std::filesystem::path &path);

  public:
    FileFilter(std::filesystem::path &root_path);

    std::vector<std::filesystem::path> filterOut(std::filesystem::path &path);

    static std::vector<std::filesystem::path>
    filterOut(std::vector<FileFilter> &filters, std::filesystem::path &path);

    std::vector<std::filesystem::path> filterIn(std::filesystem::path &path);

    static std::vector<std::filesystem::path>
    filterIn(std::vector<FileFilter> &filters, std::filesystem::path &path);

    FileFilter &addExtension(std::string &extension);
    FileFilter &addPrefix(std::string &prefix);
    FileFilter &addPath(std::filesystem::path &path);
    FileFilter &addPaths(std::initializer_list<std::filesystem::path> paths);
    FileFilter &addExtensions(std::initializer_list<std::string> extensions);
    FileFilter &addPrefixes(std::initializer_list<std::string> prefixes);
  };

} // namespace Frate::Utils
