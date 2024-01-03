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

    std::vector<GitRef> tags;
    std::vector<GitRef> branches;
    std::vector<GitRef> remotes;
    std::vector<GitRef> heads;
    std::vector<GitRef> pulls;
    std::vector<GitCommit> commits;
    bool recurse_submodules{false};
    bool no_checkout{false};
    GitStatus status_result;
    std::string raw_result;
    std::string raw_error;

  public:
    GitProvider();
    /*
     * @brief GitProvider constructor
     * @param path working directory
     */
    GitProvider(std::filesystem::path path);
    /*
     * Add command from git
     * @param pattern git add pattern
     * @return &GitProvider
     */
    GitProvider &add(std::string pattern = "-A");
    /*
     * Commit command from git
     * Requires commit message to be set
     * @return &GitProvider
     */
    GitProvider &commit();
    /*
     * Push command from git
     * @return &GitProvider
     */
    GitProvider &push();
    /*
     * Pull command from git
     * @return &GitProvider
     */
    GitProvider &pull();
    /*
     * Clone command from git
     * Requires git url to be set or passed in
     * @param url git url
     * @return &GitProvider
     */
    GitProvider &clone(std::string url = "");
    /*
     * Checkout command from git
     * Requires git url to be set or passed in
     * @param url git url
     * @return &GitProvider
     */
    GitProvider &checkout(std::string branch = "");
    /*
     * Fetch command from git
     * @return &GitProvider
     */
    GitProvider &fetch();
    /*
     * ls-remote command from git
     * Gets all the refs from the current branch and puts the data in
     * `getTags()`, `getBranches()`, `getRemotes()`, `getHeads()`, `getPulls()`
     * Requires git url to be set or passed in
     * @param url git url
     * @return &GitProvider
     */
    GitProvider &lsRemote(std::string url = "");
    /*
     * log command from git
     * Gets all the commits from the current branch and puts the data in
     * `getCommits()`
     * @return &GitProvider
     */
    GitProvider &log();
    /*
     * status command from git
     * parses the status of the current working directory and puts the data in
     * `getStatus()`
     * @return &GitProvider
     */
    GitProvider &status();
    /*
     * Intended to be chained before other commands
     * @param url git url
     * @return &GitProvider
     */
    GitProvider &setGitUrl(std::string url);
    /*
     * Intended to be chained before other commands
     * @param path working directory
     * @return &GitProvider
     */
    GitProvider &setWorkingDir(std::filesystem::path path);
    /*
     * Intended to be chained before other commands
     * @param branch git branch
     * @return &GitProvider
     */
    GitProvider &setBranch(std::string branch);
    /*
     * Intended to be chained before other commands
     * @param message git commit message
     * @return &GitProvider
     */
    GitProvider &setCommitMessage(std::string message);
    /*
     * Intended to be chained before other commands
     * @param recurse recurse submodules
     * @return &GitProvider
     */
    GitProvider &setRecurseSubmodules(bool recurse);
    /*
     * Intended to be chained before other commands
     * @param no_checkout adds --no-checkout to clone command
     * @return &GitProvider
     */
    GitProvider &setNoCheckout(bool no_checkout);

    const std::vector<GitRef> &getTags() { return tags; };

    const std::vector<GitRef> &getBranches() { return branches; };

    const std::vector<GitRef> &getRemotes() { return remotes; };

    const std::vector<GitRef> &getHeads() { return heads; };

    const std::vector<GitRef> &getPulls() { return pulls; };

    const std::vector<GitCommit> &getCommits() { return commits; };

    const GitStatus &getStatus() { return status_result; };

    const std::string &getRawResult() { return raw_result; };

    const std::string &getRawError() { return raw_error; };

    friend std::ostream &operator<<(std::ostream &os_stream,
                                    const GitProvider &git);
  };
} // namespace Frate::System
