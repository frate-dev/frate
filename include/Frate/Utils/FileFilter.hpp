#include <Frate/Frate.hpp>
#include <filesystem>
#include <vector>

namespace Frate::Utils {
  class FileFilterException : public FrateException {
  public:
    FileFilterException(const std::string &message) : FrateException(message) {}
  };
  class FileFilter {
  private:
    std::vector<std::string> filter_extensions;
    std::vector<std::string> filter_prefixes;
    std::vector<std::string> filter_files;
    std::vector<std::filesystem::path> filter_dirs;

    std::filesystem::path root_path;
    std::vector<std::filesystem::path> paths_to_filter;
    std::vector<std::filesystem::path> filtered_paths;

    bool paths_generated = false;


    bool is_valid(const std::filesystem::path &path);

    void gen_paths();

  public:
    FileFilter(std::filesystem::path &root_path) : root_path(root_path) {}

    FileFilter(std::vector<std::filesystem::path> &paths) : paths_to_filter(paths) {
      paths_generated = true;
    }

    std::vector<std::filesystem::path> filterOut();

    static std::vector<std::filesystem::path>
    filterOut(std::vector<FileFilter> &filters, std::filesystem::path &path);

    std::vector<std::filesystem::path> filterIn();

    static std::vector<std::filesystem::path>
    filterIn(std::vector<FileFilter> &filters, std::filesystem::path &path);

    FileFilter &addExtension(std::string &extension);
    FileFilter &addExtensions(std::initializer_list<std::string> extensions);
    FileFilter &addPrefix(std::string &prefix);
    FileFilter &addPrefixes(std::initializer_list<std::string> prefixes);
    FileFilter &addPath(std::filesystem::path &path);
    FileFilter &addDirs(std::initializer_list<std::filesystem::path> paths);
    FileFilter &addFile(std::string &file);
    FileFilter &addFiles(std::initializer_list<std::string> files);
  };

} // namespace Frate::Utils
