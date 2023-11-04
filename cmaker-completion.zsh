#compdef cmaker

_cmaker() {
  local state
  local -a options
  local -a cmd

  _arguments -C \
    '1: :->command' \
    '2: :->subcommand' \
    '*::arg:->args'

  options=(
    'init:Initializes your project'
    'run:Builds and runs your project'
    'watch:Watches builds and runs your project on changes'
    'add:[dep, lib, flags] Add library, dependecy or flags to your project'
    'ftp:Delets the entire project (F*ck this project)'
    'help:Print help'
  )

  add_options=(
    'dep:Add a dependecy <dep>'
    'lib:Add a library <lib>'
    'flags:Add a flag <flag>'
  )

  case $state in 
    command)
      _describe -t commands "command" options
      ;;
    subcommand)
      case $words[2] in
        add)
        _describe -t subcommands "subcommand" add_options
      esac
      ;;
    args)
      # Leaving empty for now
      ;;
  esac
}

_cmaker "$@"
