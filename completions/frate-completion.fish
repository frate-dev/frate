function __fish_frate_needs_command
    set -l cmd (commandline -opc)
    if test (count $cmd) -eq 1
        return 0
    end
    return 1
end

function __fish_frate_using_command
    set -l cmd (commandline -opc)
    set -e cmd[1]
    if contains -- $argv[1] $cmd
        return 0
    end
    return 1
end

function __fish_frate_needs_subcommand
    set -l cmd (commandline -opc)
    if test (count $cmd) -eq 2
        return 0
    end
    return 1
end

complete -c frate -n '__fish_cmaker_needs_command' -a 'init run watch add ftp help' -f
complete -c frate -n '__fish_cmaker_using_command add' -a 'dep lib flags' -f
