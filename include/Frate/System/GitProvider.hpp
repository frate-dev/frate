#pragma once
#include <Frate/System/GitCommit.hpp>
#include <Frate/System/GitRef.hpp>
#include <Frate/System/GitStatus.hpp>
#include <filesystem>
#include <string>
#include <vector>

namespace Frate::System {

  enum ArchiveType { TAR, ZIP };

  const std::ostream &operator<<(std::ostream &os_stream,
                                 const ArchiveType &type);

  class GitProvider {
  private:
    std::string git_url;
    std::filesystem::path working_dir;
    std::string branch;
    std::string commit_message;
    std::string work_dir_cmd();
    GitRef parse_ref(std::string &line);
    std::vector<GitRef> parse_refs(std::string &lines);
    GitCommit parse_commit(std::string &line);
    std::vector<GitCommit> parse_commits(std::string &lines);

  public:
    std::vector<GitRef> tags;
    std::vector<GitRef> branches;
    std::vector<GitRef> remotes;
    std::vector<GitRef> heads;
    std::vector<GitRef> pulls;
    std::vector<GitCommit> commits;
    GitStatus status_result;
    std::string raw_result;
    std::string raw_error;

    GitProvider();

    GitProvider &add(std::string pattern = "-A");
    GitProvider &commit();
    GitProvider &push();
    GitProvider &pull();
    GitProvider &clone();
    GitProvider &checkout();
    GitProvider &fetch();
    GitProvider &lsRemote();
    GitProvider &log();
    GitProvider &status();
    GitProvider &downloadArchive(ArchiveType type);

    GitProvider &setGitUrl(std::string url);

    GitProvider &setWorkingDir(std::filesystem::path path);

    GitProvider &setBranch(std::string branch);

    GitProvider &setCommitMessage(std::string message);

    friend std::ostream &operator<<(std::ostream &os_stream,
                                    const GitProvider &git);
  };
} // namespace Frate::System
