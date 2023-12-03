DEBUG MODE ENABLED
Loading: "/home/deastl/repos/cpp/frate/build/frate-project.json"
loadedJson = {"authors":[],"build_dir":"build","cmake_version":"3.10","compiler":"g++","default_mode":"Release","dependencies":[],"flags":[],"include_dir":"include","keywords":[],"lang":"cpp","lang_version":"20","modes":[{"dependencies":[],"flags":["-O2 "],"name":"Release"},{"dependencies":[],"flags":["-g"],"name":"Debug"},{"dependencies":[],"flags":["-g"],"name":"Test"}],"project_description":"this is the description of the project and I really like this","project_name":"debug-frate","project_path":"/home/deastl/repos/cpp/frate/build","project_type":"header_only","project_version":"0.0.1","src_dir":"src","toolchains":[]}
Project Path: "/home/deastl/repos/cpp/frate/build"
#compdef frate
_frate() {
options=(
  new: "Create a new project"
  run: "Run the project"
  help: "Display help"
  nuke: "Deletes the entire project"
  add: "add sub command"
  set: "set sub command"
  search: "search sub command"
  list: "list sub command"
  remove: "remove sub command"
  update: "update sub command"
  clean: "clean sub command"
  build: "build sub command"
  completions: "completions sub command"
  watch: "watches the project for changes"
)
sub_options=(
)
  local -a commands
  commands=( new run help nuke add set search list remove update clean build completions watch )
  local -a subcommands
  subcommands=( packages flags lib mode server toolchain author keywords license name version server package modes servers installed-packages flags authors available-targets licenses packages flag mode author index )
  _arguments -C \
    '1: :->command' \
    '2: :->subcommand' \
    '3: :->args' \
    && case $state in
      command)
        _describe -t commands 'command' commands
      ;;
      subcommand)
        _describe -t subcommands 'subcommand' subcommands
      ;;
      args)
        case $words[1] in
          new)
    options=(
      new: "Create a new project"
      run: "Run the project"
      help: "Display help"
      nuke: "Deletes the entire project"
      add: "add sub command"
      set: "set sub command"
      search: "search sub command"
      list: "list sub command"
      remove: "remove sub command"
      update: "update sub command"
      clean: "clean sub command"
      build: "build sub command"
      completions: "completions sub command"
      watch: "watches the project for changes"
    )
    sub_options=(
    )
            _describe -t options 'options' options
            _describe -t sub_options 'sub_options' sub_options
          ;;
          run)
    options=(
      new: "Create a new project"
      run: "Run the project"
      help: "Display help"
      nuke: "Deletes the entire project"
      add: "add sub command"
      set: "set sub command"
      search: "search sub command"
      list: "list sub command"
      remove: "remove sub command"
      update: "update sub command"
      clean: "clean sub command"
      build: "build sub command"
      completions: "completions sub command"
      watch: "watches the project for changes"
    )
    sub_options=(
    )
            _describe -t options 'options' options
            _describe -t sub_options 'sub_options' sub_options
          ;;
          help)
    options=(
      new: "Create a new project"
      run: "Run the project"
      help: "Display help"
      nuke: "Deletes the entire project"
      add: "add sub command"
      set: "set sub command"
      search: "search sub command"
      list: "list sub command"
      remove: "remove sub command"
      update: "update sub command"
      clean: "clean sub command"
      build: "build sub command"
      completions: "completions sub command"
      watch: "watches the project for changes"
    )
    sub_options=(
    )
            _describe -t options 'options' options
            _describe -t sub_options 'sub_options' sub_options
          ;;
          nuke)
    options=(
      new: "Create a new project"
      run: "Run the project"
      help: "Display help"
      nuke: "Deletes the entire project"
      add: "add sub command"
      set: "set sub command"
      search: "search sub command"
      list: "list sub command"
      remove: "remove sub command"
      update: "update sub command"
      clean: "clean sub command"
      build: "build sub command"
      completions: "completions sub command"
      watch: "watches the project for changes"
    )
    sub_options=(
    )
            _describe -t options 'options' options
            _describe -t sub_options 'sub_options' sub_options
          ;;
          add)
    options=(
      new: "Create a new project"
      run: "Run the project"
      help: "Display help"
      nuke: "Deletes the entire project"
      add: "add sub command"
      set: "set sub command"
      search: "search sub command"
      list: "list sub command"
      remove: "remove sub command"
      update: "update sub command"
      clean: "clean sub command"
      build: "build sub command"
      completions: "completions sub command"
      watch: "watches the project for changes"
    )
    sub_options=(
      packages: "Add a package to the project"
      flags: "Add a flag to the project"
      lib: "Add a library to link to your project"
      mode: "Adds a build mode to your project"
      server: "Add a remote server to your local config that you can later build to"
      toolchain: "Add a crosscompile toolchain to your project"
      author: "Add an author to your project"
      keywords: "Add keywords to your project"
    )
            _describe -t options 'options' options
            _describe -t sub_options 'sub_options' sub_options
          ;;
          set)
    options=(
      new: "Create a new project"
      run: "Run the project"
      help: "Display help"
      nuke: "Deletes the entire project"
      add: "add sub command"
      set: "set sub command"
      search: "search sub command"
      list: "list sub command"
      remove: "remove sub command"
      update: "update sub command"
      clean: "clean sub command"
      build: "build sub command"
      completions: "completions sub command"
      watch: "watches the project for changes"
    )
    sub_options=(
      license: "Set the project's license"
      name: "Set the project's name"
      version: "Set the project's version"
      server: "Set the project's build server"
    )
            _describe -t options 'options' options
            _describe -t sub_options 'sub_options' sub_options
          ;;
          search)
    options=(
      new: "Create a new project"
      run: "Run the project"
      help: "Display help"
      nuke: "Deletes the entire project"
      add: "add sub command"
      set: "set sub command"
      search: "search sub command"
      list: "list sub command"
      remove: "remove sub command"
      update: "update sub command"
      clean: "clean sub command"
      build: "build sub command"
      completions: "completions sub command"
      watch: "watches the project for changes"
    )
    sub_options=(
      package: "Search for packages"
    )
            _describe -t options 'options' options
            _describe -t sub_options 'sub_options' sub_options
          ;;
          list)
    options=(
      new: "Create a new project"
      run: "Run the project"
      help: "Display help"
      nuke: "Deletes the entire project"
      add: "add sub command"
      set: "set sub command"
      search: "search sub command"
      list: "list sub command"
      remove: "remove sub command"
      update: "update sub command"
      clean: "clean sub command"
      build: "build sub command"
      completions: "completions sub command"
      watch: "watches the project for changes"
    )
    sub_options=(
      modes: "List modes"
      servers: "List remote servers"
      installed-packages: "List installed packages"
      flags: "List flags"
      authors: "List authors"
      available-targets: "List available targets"
      licenses: "List possible licenses"
    )
            _describe -t options 'options' options
            _describe -t sub_options 'sub_options' sub_options
          ;;
          remove)
    options=(
      new: "Create a new project"
      run: "Run the project"
      help: "Display help"
      nuke: "Deletes the entire project"
      add: "add sub command"
      set: "set sub command"
      search: "search sub command"
      list: "list sub command"
      remove: "remove sub command"
      update: "update sub command"
      clean: "clean sub command"
      build: "build sub command"
      completions: "completions sub command"
      watch: "watches the project for changes"
    )
    sub_options=(
      packages: "Remove a package from the project"
      flag: "Remove a flag from the project"
      mode: "Remove a mode from the project"
      author: "Remove an author from the project"
    )
            _describe -t options 'options' options
            _describe -t sub_options 'sub_options' sub_options
          ;;
          update)
    options=(
      new: "Create a new project"
      run: "Run the project"
      help: "Display help"
      nuke: "Deletes the entire project"
      add: "add sub command"
      set: "set sub command"
      search: "search sub command"
      list: "list sub command"
      remove: "remove sub command"
      update: "update sub command"
      clean: "clean sub command"
      build: "build sub command"
      completions: "completions sub command"
      watch: "watches the project for changes"
    )
    sub_options=(
      index: "Update package index"
    )
            _describe -t options 'options' options
            _describe -t sub_options 'sub_options' sub_options
          ;;
          clean)
    options=(
      new: "Create a new project"
      run: "Run the project"
      help: "Display help"
      nuke: "Deletes the entire project"
      add: "add sub command"
      set: "set sub command"
      search: "search sub command"
      list: "list sub command"
      remove: "remove sub command"
      update: "update sub command"
      clean: "clean sub command"
      build: "build sub command"
      completions: "completions sub command"
      watch: "watches the project for changes"
    )
    sub_options=(
    )
            _describe -t options 'options' options
            _describe -t sub_options 'sub_options' sub_options
          ;;
          build)
    options=(
      new: "Create a new project"
      run: "Run the project"
      help: "Display help"
      nuke: "Deletes the entire project"
      add: "add sub command"
      set: "set sub command"
      search: "search sub command"
      list: "list sub command"
      remove: "remove sub command"
      update: "update sub command"
      clean: "clean sub command"
      build: "build sub command"
      completions: "completions sub command"
      watch: "watches the project for changes"
    )
    sub_options=(
    )
            _describe -t options 'options' options
            _describe -t sub_options 'sub_options' sub_options
          ;;
          completions)
    options=(
      new: "Create a new project"
      run: "Run the project"
      help: "Display help"
      nuke: "Deletes the entire project"
      add: "add sub command"
      set: "set sub command"
      search: "search sub command"
      list: "list sub command"
      remove: "remove sub command"
      update: "update sub command"
      clean: "clean sub command"
      build: "build sub command"
      completions: "completions sub command"
      watch: "watches the project for changes"
    )
    sub_options=(
    )
            _describe -t options 'options' options
            _describe -t sub_options 'sub_options' sub_options
          ;;
          watch)
    options=(
      new: "Create a new project"
      run: "Run the project"
      help: "Display help"
      nuke: "Deletes the entire project"
      add: "add sub command"
      set: "set sub command"
      search: "search sub command"
      list: "list sub command"
      remove: "remove sub command"
      update: "update sub command"
      clean: "clean sub command"
      build: "build sub command"
      completions: "completions sub command"
      watch: "watches the project for changes"
    )
    sub_options=(
    )
            _describe -t options 'options' options
            _describe -t sub_options 'sub_options' sub_options
          ;;
