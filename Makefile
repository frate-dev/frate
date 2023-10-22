# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/deastl/repos/cmake-generator

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/deastl/repos/cmake-generator

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/deastl/repos/cmake-generator/CMakeFiles /home/deastl/repos/cmake-generator//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/deastl/repos/cmake-generator/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named cmake-generator

# Build rule for target.
cmake-generator: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 cmake-generator
.PHONY : cmake-generator

# fast build rule for target.
cmake-generator/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/build
.PHONY : cmake-generator/fast

src/Command/CommandFTP.o: src/Command/CommandFTP.cpp.o
.PHONY : src/Command/CommandFTP.o

# target to build an object file
src/Command/CommandFTP.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/Command/CommandFTP.cpp.o
.PHONY : src/Command/CommandFTP.cpp.o

src/Command/CommandFTP.i: src/Command/CommandFTP.cpp.i
.PHONY : src/Command/CommandFTP.i

# target to preprocess a source file
src/Command/CommandFTP.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/Command/CommandFTP.cpp.i
.PHONY : src/Command/CommandFTP.cpp.i

src/Command/CommandFTP.s: src/Command/CommandFTP.cpp.s
.PHONY : src/Command/CommandFTP.s

# target to generate assembly for a file
src/Command/CommandFTP.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/Command/CommandFTP.cpp.s
.PHONY : src/Command/CommandFTP.cpp.s

src/Command/CommandFlags.o: src/Command/CommandFlags.cpp.o
.PHONY : src/Command/CommandFlags.o

# target to build an object file
src/Command/CommandFlags.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/Command/CommandFlags.cpp.o
.PHONY : src/Command/CommandFlags.cpp.o

src/Command/CommandFlags.i: src/Command/CommandFlags.cpp.i
.PHONY : src/Command/CommandFlags.i

# target to preprocess a source file
src/Command/CommandFlags.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/Command/CommandFlags.cpp.i
.PHONY : src/Command/CommandFlags.cpp.i

src/Command/CommandFlags.s: src/Command/CommandFlags.cpp.s
.PHONY : src/Command/CommandFlags.s

# target to generate assembly for a file
src/Command/CommandFlags.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/Command/CommandFlags.cpp.s
.PHONY : src/Command/CommandFlags.cpp.s

src/Command/CommandGeneral.o: src/Command/CommandGeneral.cpp.o
.PHONY : src/Command/CommandGeneral.o

# target to build an object file
src/Command/CommandGeneral.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/Command/CommandGeneral.cpp.o
.PHONY : src/Command/CommandGeneral.cpp.o

src/Command/CommandGeneral.i: src/Command/CommandGeneral.cpp.i
.PHONY : src/Command/CommandGeneral.i

# target to preprocess a source file
src/Command/CommandGeneral.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/Command/CommandGeneral.cpp.i
.PHONY : src/Command/CommandGeneral.cpp.i

src/Command/CommandGeneral.s: src/Command/CommandGeneral.cpp.s
.PHONY : src/Command/CommandGeneral.s

# target to generate assembly for a file
src/Command/CommandGeneral.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/Command/CommandGeneral.cpp.s
.PHONY : src/Command/CommandGeneral.cpp.s

src/Command/CommandHelp.o: src/Command/CommandHelp.cpp.o
.PHONY : src/Command/CommandHelp.o

# target to build an object file
src/Command/CommandHelp.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/Command/CommandHelp.cpp.o
.PHONY : src/Command/CommandHelp.cpp.o

src/Command/CommandHelp.i: src/Command/CommandHelp.cpp.i
.PHONY : src/Command/CommandHelp.i

# target to preprocess a source file
src/Command/CommandHelp.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/Command/CommandHelp.cpp.i
.PHONY : src/Command/CommandHelp.cpp.i

src/Command/CommandHelp.s: src/Command/CommandHelp.cpp.s
.PHONY : src/Command/CommandHelp.s

# target to generate assembly for a file
src/Command/CommandHelp.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/Command/CommandHelp.cpp.s
.PHONY : src/Command/CommandHelp.cpp.s

src/Command/CommandInit.o: src/Command/CommandInit.cpp.o
.PHONY : src/Command/CommandInit.o

# target to build an object file
src/Command/CommandInit.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/Command/CommandInit.cpp.o
.PHONY : src/Command/CommandInit.cpp.o

src/Command/CommandInit.i: src/Command/CommandInit.cpp.i
.PHONY : src/Command/CommandInit.i

# target to preprocess a source file
src/Command/CommandInit.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/Command/CommandInit.cpp.i
.PHONY : src/Command/CommandInit.cpp.i

src/Command/CommandInit.s: src/Command/CommandInit.cpp.s
.PHONY : src/Command/CommandInit.s

# target to generate assembly for a file
src/Command/CommandInit.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/Command/CommandInit.cpp.s
.PHONY : src/Command/CommandInit.cpp.s

src/Command/CommandLib.o: src/Command/CommandLib.cpp.o
.PHONY : src/Command/CommandLib.o

# target to build an object file
src/Command/CommandLib.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/Command/CommandLib.cpp.o
.PHONY : src/Command/CommandLib.cpp.o

src/Command/CommandLib.i: src/Command/CommandLib.cpp.i
.PHONY : src/Command/CommandLib.i

# target to preprocess a source file
src/Command/CommandLib.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/Command/CommandLib.cpp.i
.PHONY : src/Command/CommandLib.cpp.i

src/Command/CommandLib.s: src/Command/CommandLib.cpp.s
.PHONY : src/Command/CommandLib.s

# target to generate assembly for a file
src/Command/CommandLib.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/Command/CommandLib.cpp.s
.PHONY : src/Command/CommandLib.cpp.s

src/Command/CommandRun.o: src/Command/CommandRun.cpp.o
.PHONY : src/Command/CommandRun.o

# target to build an object file
src/Command/CommandRun.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/Command/CommandRun.cpp.o
.PHONY : src/Command/CommandRun.cpp.o

src/Command/CommandRun.i: src/Command/CommandRun.cpp.i
.PHONY : src/Command/CommandRun.i

# target to preprocess a source file
src/Command/CommandRun.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/Command/CommandRun.cpp.i
.PHONY : src/Command/CommandRun.cpp.i

src/Command/CommandRun.s: src/Command/CommandRun.cpp.s
.PHONY : src/Command/CommandRun.s

# target to generate assembly for a file
src/Command/CommandRun.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/Command/CommandRun.cpp.s
.PHONY : src/Command/CommandRun.cpp.s

src/main.o: src/main.cpp.o
.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i
.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s
.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/cmake-generator.dir/build.make CMakeFiles/cmake-generator.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... cmake-generator"
	@echo "... src/Command/CommandFTP.o"
	@echo "... src/Command/CommandFTP.i"
	@echo "... src/Command/CommandFTP.s"
	@echo "... src/Command/CommandFlags.o"
	@echo "... src/Command/CommandFlags.i"
	@echo "... src/Command/CommandFlags.s"
	@echo "... src/Command/CommandGeneral.o"
	@echo "... src/Command/CommandGeneral.i"
	@echo "... src/Command/CommandGeneral.s"
	@echo "... src/Command/CommandHelp.o"
	@echo "... src/Command/CommandHelp.i"
	@echo "... src/Command/CommandHelp.s"
	@echo "... src/Command/CommandInit.o"
	@echo "... src/Command/CommandInit.i"
	@echo "... src/Command/CommandInit.s"
	@echo "... src/Command/CommandLib.o"
	@echo "... src/Command/CommandLib.i"
	@echo "... src/Command/CommandLib.s"
	@echo "... src/Command/CommandRun.o"
	@echo "... src/Command/CommandRun.i"
	@echo "... src/Command/CommandRun.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

