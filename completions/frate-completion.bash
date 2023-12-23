_frate() {
    local cur prev words cword
    _get_comp_words_by_ref -n : cur prev words cword
    local commands="init run watch add ftp help"
    local add_subcommands="dep lib flags"

    case "${prev}" in
        add)
            COMPREPLY=($(compgen -W "${add_subcommands}" -- "${cur}"))
            return 0
            ;;
    esac

    case "${words[1]}" in
        add)
            if [ "$cword" -eq 2 ]; then
                COMPREPLY=($(compgen -W "${add_subcommands}" -- "${cur}"))
                return 0
            fi
            ;;
        *)
            if [ "$cword" -eq 1 ]; then
                COMPREPLY=($(compgen -W "${commands}" -- "${cur}"))
                return 0
            fi
            ;;
    esac

    COMPREPLY=()
}

complete -F _frate cmaker
