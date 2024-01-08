#include "Frate/Utils/Logging.hpp"
#include <Frate/Utils/FileFilter.hpp>
#include <filesystem>

namespace Frate::Utils {
  using std::filesystem::path;

  FileFilter::FileFilter(std::filesystem::path &root_path) {
    this->root_path = root_path;
  }

  bool FileFilter::is_valid(const std::filesystem::path &check_path) {
    if (!std::filesystem::exists(check_path)) {
      throw std::runtime_error("Path does not exist");
    }
    for (std::string &extension : extensions) {
      if (check_path.extension() == extension) {
        return true;
      }
    }
    for (std::string &prefix : prefixes) {
      if (check_path.filename().string().find(prefix) != std::string::npos) {
        return true;
      }
    }
    for (path &curr_path : paths) {
      if (check_path.string().find(root_path.string() + "/" +
                                   curr_path.string()) != std::string::npos) {
        return true;
      }
    }
    return false;
  }

  std::vector<std::filesystem::path>
  FileFilter::filterOut(std::filesystem::path &root_path) {

    std::vector<std::filesystem::path> filtered_paths{};

    if (!std::filesystem::exists(root_path)) {
      throw std::runtime_error("Path does not exist");
    }

    for (std::filesystem::directory_entry entry :
         std::filesystem::recursive_directory_iterator(root_path)) {
      if (!is_valid(entry.path())) {
        filtered_paths.emplace_back(entry.path());
      }
    }
    return filtered_paths;
  }

  std::vector<std::filesystem::path>
  FileFilter::filterOut(std::vector<FileFilter> &filters,
                        std::filesystem::path &root_path) {

    std::vector<std::filesystem::path> filtered_paths{};

    for (std::filesystem::directory_entry entry :
         std::filesystem::recursive_directory_iterator(root_path)) {
      for (FileFilter &filter : filters) {
        if (!filter.is_valid(entry.path())) {
          filtered_paths.emplace_back(entry.path());
        }
      }
    }
    return filtered_paths;
  }

  std::vector<std::filesystem::path>
  FileFilter::filterIn(std::filesystem::path &root_path) {

    std::vector<std::filesystem::path> filtered_paths{};
    for (std::filesystem::directory_entry entry :
         std::filesystem::recursive_directory_iterator(root_path)) {
      if (is_valid(entry.path())) {
        filtered_paths.emplace_back(entry.path());
      }
    }
    return filtered_paths;
  }

  std::vector<std::filesystem::path>
  FileFilter::filterIn(std::vector<FileFilter> &filters,
                       std::filesystem::path &root_path) {

    std::vector<std::filesystem::path> filtered_paths{};
    for (std::filesystem::directory_entry entry :
         std::filesystem::recursive_directory_iterator(root_path)) {
      for (FileFilter &filter : filters) {
        if (filter.is_valid(entry.path())) {
          filtered_paths.emplace_back(entry.path());
        }
      }
    }
    return filtered_paths;
  };

  FileFilter &FileFilter::addExtension(std::string &extension) {
    extensions.emplace_back(extension);
    return *this;
  }

  FileFilter &FileFilter::addPrefix(std::string &prefix) {
    prefixes.emplace_back(prefix);
    return *this;
  }

  FileFilter &FileFilter::addPath(std::filesystem::path &path) {
    paths.emplace_back(path);
    return *this;
  }

  FileFilter &
  FileFilter::addPaths(std::initializer_list<std::filesystem::path> paths) {
    for (std::filesystem::path path : paths) {
      this->paths.emplace_back(path);
    }
    return *this;
  }

  FileFilter &
  FileFilter::addExtensions(std::initializer_list<std::string> extensions) {
    for (std::string extension : extensions) {
      this->extensions.emplace_back(extension);
    }
    return *this;
  }

  FileFilter &
  FileFilter::addPrefixes(std::initializer_list<std::string> prefixes) {
    for (std::string prefix : prefixes) {
      this->prefixes.emplace_back(prefix);
    }
    return *this;
  }

} // namespace Frate::Utils
