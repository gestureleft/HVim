# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/harrisonmarshall/dev/HVim

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/harrisonmarshall/dev/HVim

# Include any dependencies generated for this target.
include hvim_tests/CMakeFiles/hvimtestsrun.dir/depend.make

# Include the progress variables for this target.
include hvim_tests/CMakeFiles/hvimtestsrun.dir/progress.make

# Include the compile flags for this target's objects.
include hvim_tests/CMakeFiles/hvimtestsrun.dir/flags.make

hvim_tests/CMakeFiles/hvimtestsrun.dir/MoveCursorTest.cpp.o: hvim_tests/CMakeFiles/hvimtestsrun.dir/flags.make
hvim_tests/CMakeFiles/hvimtestsrun.dir/MoveCursorTest.cpp.o: hvim_tests/MoveCursorTest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/harrisonmarshall/dev/HVim/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object hvim_tests/CMakeFiles/hvimtestsrun.dir/MoveCursorTest.cpp.o"
	cd /Users/harrisonmarshall/dev/HVim/hvim_tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/hvimtestsrun.dir/MoveCursorTest.cpp.o -c /Users/harrisonmarshall/dev/HVim/hvim_tests/MoveCursorTest.cpp

hvim_tests/CMakeFiles/hvimtestsrun.dir/MoveCursorTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hvimtestsrun.dir/MoveCursorTest.cpp.i"
	cd /Users/harrisonmarshall/dev/HVim/hvim_tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/harrisonmarshall/dev/HVim/hvim_tests/MoveCursorTest.cpp > CMakeFiles/hvimtestsrun.dir/MoveCursorTest.cpp.i

hvim_tests/CMakeFiles/hvimtestsrun.dir/MoveCursorTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hvimtestsrun.dir/MoveCursorTest.cpp.s"
	cd /Users/harrisonmarshall/dev/HVim/hvim_tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/harrisonmarshall/dev/HVim/hvim_tests/MoveCursorTest.cpp -o CMakeFiles/hvimtestsrun.dir/MoveCursorTest.cpp.s

# Object files for target hvimtestsrun
hvimtestsrun_OBJECTS = \
"CMakeFiles/hvimtestsrun.dir/MoveCursorTest.cpp.o"

# External object files for target hvimtestsrun
hvimtestsrun_EXTERNAL_OBJECTS =

hvim_tests/hvimtestsrun: hvim_tests/CMakeFiles/hvimtestsrun.dir/MoveCursorTest.cpp.o
hvim_tests/hvimtestsrun: hvim_tests/CMakeFiles/hvimtestsrun.dir/build.make
hvim_tests/hvimtestsrun: lib/libgtest.a
hvim_tests/hvimtestsrun: lib/libgtest_main.a
hvim_tests/hvimtestsrun: lib/libgtest.a
hvim_tests/hvimtestsrun: hvim_tests/CMakeFiles/hvimtestsrun.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/harrisonmarshall/dev/HVim/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable hvimtestsrun"
	cd /Users/harrisonmarshall/dev/HVim/hvim_tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hvimtestsrun.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
hvim_tests/CMakeFiles/hvimtestsrun.dir/build: hvim_tests/hvimtestsrun

.PHONY : hvim_tests/CMakeFiles/hvimtestsrun.dir/build

hvim_tests/CMakeFiles/hvimtestsrun.dir/clean:
	cd /Users/harrisonmarshall/dev/HVim/hvim_tests && $(CMAKE_COMMAND) -P CMakeFiles/hvimtestsrun.dir/cmake_clean.cmake
.PHONY : hvim_tests/CMakeFiles/hvimtestsrun.dir/clean

hvim_tests/CMakeFiles/hvimtestsrun.dir/depend:
	cd /Users/harrisonmarshall/dev/HVim && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/harrisonmarshall/dev/HVim /Users/harrisonmarshall/dev/HVim/hvim_tests /Users/harrisonmarshall/dev/HVim /Users/harrisonmarshall/dev/HVim/hvim_tests /Users/harrisonmarshall/dev/HVim/hvim_tests/CMakeFiles/hvimtestsrun.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : hvim_tests/CMakeFiles/hvimtestsrun.dir/depend

