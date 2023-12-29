#include "Frate/Utils/General.hpp"
#include <Frate/System/GitException.hpp>
#include <Frate/System/GitProvider.hpp>

namespace Frate::System {
  GitProvider::GitProvider() {
    this->working_dir = std::filesystem::current_path();
    this->branch = "main";
    this->commit_message = "Frate commit";
  };

  const std::ostream &operator<<(std::ostream &os_stream,
                                 const ArchiveType &type) {
    switch (type) {
    case TAR:
      os_stream << "tar";
      break;
    case ZIP:
      os_stream << "zip";
      break;
    }
    return os_stream;
  }

  GitProvider &GitProvider::add(std::string pattern) {
    Utils::CmdOutput out =
        Utils::hSystemWithOutput(work_dir_cmd() + "git add " + pattern);
    this->raw_result = out.std_out;
    this->raw_error = out.std_err;

    if (!this->raw_error.empty()) {
      throw GitException(this->raw_error);
    }

    return *this;
  }

  GitProvider &GitProvider::commit() {
    Utils::CmdOutput out = Utils::hSystemWithOutput(
        work_dir_cmd() + "git commit -m '" + this->commit_message + "'");
    this->raw_result = out.std_out;
    this->raw_error = out.std_err;

    if (!this->raw_error.empty()) {
      throw GitException(this->raw_error);
    }

    return *this;
  }

  GitProvider &GitProvider::push() {

    Utils::CmdOutput out =
        Utils::hSystemWithOutput(work_dir_cmd() + "git push origin " +
                                 this->branch + " --force-with-lease");

    this->raw_result = out.std_out;
    this->raw_error = out.std_err;

    if (!this->raw_error.empty()) {
      throw GitException(this->raw_error);
    }

    return *this;
  }

  GitProvider &GitProvider::pull() { return *this; }

  GitProvider &GitProvider::clone() {
    if (this->git_url.empty()) {
      throw std::runtime_error("Git url is empty");
    }
    Utils::CmdOutput out = Utils::hSystemWithOutput(
        "git clone " + this->git_url + " " + this->working_dir.string());
    this->raw_result = out.std_out;
    this->raw_error = out.std_err;

    if (!this->raw_error.empty()) {
      throw GitException(this->raw_error);
    }

    return *this;
  }

  GitProvider &GitProvider::checkout() { return *this; }

  GitProvider &GitProvider::fetch() {
    Utils::CmdOutput out =
        Utils::hSystemWithOutput(work_dir_cmd() + "git fetch");
    this->raw_result = out.std_out;
    this->raw_error = out.std_err;

    if (!this->raw_error.empty()) {
      throw GitException(this->raw_error);
    }

    return *this;
  }

  GitProvider &GitProvider::lsRemote() {

    Utils::CmdOutput out =
        Utils::hSystemWithOutput(work_dir_cmd() + "git --no-pager ls-remote");

    this->raw_result = out.std_out;
    this->raw_error = out.std_err;
    std::vector<GitRef> refs = parse_refs(out.std_out);
    /*
     * sorts refs into their respective vectors
     */
    for (auto &ref : refs) {
      if (ref.name.find("refs/tags/") != std::string::npos) {
        this->tags.push_back(ref);
      }
      else if (ref.name.find("refs/heads/") != std::string::npos) {
        this->branches.push_back(ref);
      }
      else if (ref.name.find("refs/remotes/") != std::string::npos) {
        this->remotes.push_back(ref);
      }
      else if (ref.name.find("refs/heads/") != std::string::npos) {
        this->heads.push_back(ref);
      }
      else if (ref.name.find("refs/pulls/") != std::string::npos) {
        this->pulls.push_back(ref);
      }
    }

    if (!this->raw_error.empty()) {
      throw GitException(this->raw_error);
    }
    return *this;
  }

  GitProvider &GitProvider::log() {

    Utils::CmdOutput out = Utils::hSystemWithOutput(
        work_dir_cmd() +
        "git --no-pager log --format='hash:%H name:%an date:%ad message:%s'");

    this->raw_result = out.std_out;
    this->raw_error = out.std_err;

    std::vector<GitCommit> commits = parse_commits(out.std_out);
    this->commits = commits;

    if (!this->raw_error.empty()) {
      throw GitException(this->raw_error);
    }
    return *this;
  }

  GitProvider &GitProvider::status() {

    Utils::CmdOutput out =
        Utils::hSystemWithOutput(work_dir_cmd() + "git status --short");

    this->raw_result = out.std_out;
    this->raw_error = out.std_err;

    if (out.std_out.empty()) {
      return *this;
    }

    std::vector<std::string> lines = Utils::split(out.std_out, '\n');

    for (auto &line : lines) {
      if (line.empty()) {
        continue;
      }
      // If the line is less than 3 characters then we know it's not a valid
      if (line.size() < 3) {
        continue;
      }
      if (line[1] == 'A') {
        this->status_result.added.push_back(line.substr(3));
      }
      else if (line[1] == 'M') {
        this->status_result.modified.push_back(line.substr(3));
      }
      else if (line[1] == 'D') {
        this->status_result.deleted.push_back(line.substr(3));
      }
      else if (line[1] == 'R') {
        this->status_result.renamed.push_back(line.substr(3));
      }
      else if (line[1] == 'C') {
        this->status_result.copied.push_back(line.substr(3));
      }
      else if (line[0] == '?') {
        this->status_result.untracked.push_back(line.substr(3));
      }
      else if (line[1] == '!') {
        this->status_result.ignored.push_back(line.substr(3));
      }
    }

    if (!this->raw_error.empty()) {
      throw GitException(this->raw_error);
    }

    return *this;
  }

  GitProvider &GitProvider::downloadArchive(ArchiveType type) { return *this; }

  /*
   * Private methods
   */

  std::string GitProvider::work_dir_cmd() {
    return "cd '" + this->working_dir.string() + "' ;";
  }

  GitRef GitProvider::parse_ref(std::string &line) {
    if (line.empty()) {
      return {};
    }
    GitRef ref;
    std::vector<std::string> parts = Utils::split(line, ' ');
    for (auto &part : parts) {
      // Parts are speperate by multiple spaces so we need to remove the empty
      if (part.empty()) {
        continue;
      }
      // if the hash is empty then we know we are on the first part
      if (ref.hash.empty()) {
        ref.hash = part;
      }
      // if the name is empty then we know we are on the second part
      else if (ref.name.empty()) {
        ref.name = part;
      }
    }
    ref.hash = parts[0];
    ref.name = parts[1];
    return ref;
  }

  std::vector<GitRef> GitProvider::parse_refs(std::string &lines) {
    if (lines.empty()) {
      return {};
    }
    std::vector<GitRef> refs;
    std::vector<std::string> lines_vec = Utils::split(lines, '\n');
    bool first_line = true;
    for (auto &line : lines_vec) {
      // Skip the first line because it's not a ref
      if (first_line) {
        first_line = false;
        continue;
      }
      refs.push_back(parse_ref(line));
    }
    return refs;
  }

  GitCommit GitProvider::parse_commit(std::string &line) {
    if (line.empty()) {
      return {};
    }
    GitCommit commit;
    std::vector<std::string> parts = Utils::split(line, ' ');
    const std::string hash_key = "hash:";
    const std::string name_key = "name:";
    const std::string date_key = "date:";
    const std::string message_key = "message:";
    for (auto &part : parts) {
      if (part.find(hash_key) != std::string::npos) {
        commit.hash = part.substr(hash_key.size());
      }
      else if (part.find(name_key) != std::string::npos) {
        commit.author = part.substr(name_key.size());
      }
      else if (part.find(date_key) != std::string::npos) {
        commit.date = part.substr(date_key.size());
      }
      else if (part.find(message_key) != std::string::npos) {
        commit.message = part.substr(message_key.size());
      }
    }
    return commit;
  }

  std::vector<GitCommit> GitProvider::parse_commits(std::string &lines) {
    if (lines.empty()) {
      return {};
    }
    std::vector<GitCommit> commits;
    std::vector<std::string> lines_vec = Utils::split(lines, '\n');
    for (auto &line : lines_vec) {
      commits.push_back(parse_commit(line));
    }
    return commits;
  }

  GitProvider &GitProvider::setGitUrl(std::string url) {
    this->git_url = url;
    return *this;
  }

  GitProvider &GitProvider::setWorkingDir(std::filesystem::path path) {
    if (!std::filesystem::exists(path)) {
      throw GitException("Path does not exist");
    }

    this->working_dir = path;
    return *this;
  }

  GitProvider &GitProvider::setBranch(std::string branch) {
    this->branch = branch;
    return *this;
  }

  GitProvider &GitProvider::setCommitMessage(std::string message) {
    this->commit_message = message;
    return *this;
  }

} // namespace Frate::System
