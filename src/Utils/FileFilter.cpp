#include "Frate/Utils/Logging.hpp"
#include <Frate/Utils/FileFilter.hpp>
#include <filesystem>

namespace Frate::Utils {
  using std::filesystem::path;

  void FileFilter::gen_paths() {

    if (paths_generated) {
      return;
    }

    if (!std::filesystem::exists(root_path)) {
      throw FileFilterException("Root path does not exist: " + root_path.string());
    }

    for (auto &entry :
         std::filesystem::recursive_directory_iterator(root_path)) {
      paths_to_filter.emplace_back(entry.path());
    }

    paths_generated = true;
  }

  bool FileFilter::is_valid(const std::filesystem::path &check_path) {
    if (!std::filesystem::exists(check_path)) {
      throw FileFilterException("Check path does not exist: " + check_path.string());
    }
    for (std::string &extension : filter_extensions) {
      if (check_path.extension() == extension) {
        return true;
      }
    }
    for (std::string &prefix : filter_prefixes) {
      if (check_path.filename().string().find(prefix) != std::string::npos) {
        return true;
      }
    }
    for (path &curr_path : filter_dirs) {

      if (check_path.string().find(root_path.string() + "/" +
                                   curr_path.string()) != std::string::npos) {
        return true;
      }
    }
    for (std::string &file : filter_files) {
      if (check_path.filename().string() == file) {
        return true;
      }
    }
    return false;
  }

  std::vector<std::filesystem::path>

  FileFilter::filterOut() {

    gen_paths();

    std::vector<std::filesystem::path> filtered_paths{};

    for(auto &path : paths_to_filter){
      if(!is_valid(path)){
        filtered_paths.emplace_back(path);
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

  std::vector<std::filesystem::path> FileFilter::filterIn() {

    gen_paths();

    for(auto &path : paths_to_filter){
      if(is_valid(path)){
        filtered_paths.emplace_back(path);
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
    filter_extensions.emplace_back(extension);
    return *this;
  }

  FileFilter &FileFilter::addPrefix(std::string &prefix) {
    filter_prefixes.emplace_back(prefix);
    return *this;
  }

  FileFilter &FileFilter::addPath(std::filesystem::path &path) {
    filter_dirs.emplace_back(path);
    return *this;
  }

  FileFilter &
  FileFilter::addDirs(std::initializer_list<std::filesystem::path> paths) {
    for (std::filesystem::path path : paths) {
      this->filter_dirs.emplace_back(path);
    }
    return *this;
  }

  FileFilter &
  FileFilter::addExtensions(std::initializer_list<std::string> extensions) {
    for (std::string extension : extensions) {
      this->filter_extensions.emplace_back(extension);
    }
    return *this;
  }

  FileFilter &
  FileFilter::addPrefixes(std::initializer_list<std::string> prefixes) {
    for (std::string prefix : prefixes) {
      this->filter_prefixes.emplace_back(prefix);
    }
    return *this;
  }

  FileFilter &FileFilter::addFile(std::string &file) {
    this->filter_files.emplace_back(file);
    return *this;
  }

  FileFilter &FileFilter::addFiles(std::initializer_list<std::string> files) {
    for (std::string file : files) {
      this->filter_files.emplace_back(file);
    }
    return *this;
  }

} // namespace Frate::Utils
