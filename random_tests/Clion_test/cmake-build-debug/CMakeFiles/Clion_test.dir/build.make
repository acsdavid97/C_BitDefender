# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/clion-2016.3.2/bin/cmake/bin/cmake

# The command to remove a file.
RM = /usr/local/clion-2016.3.2/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dvd/git/C_BitDefender/random_tests/Clion_test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dvd/git/C_BitDefender/random_tests/Clion_test/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Clion_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Clion_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Clion_test.dir/flags.make

CMakeFiles/Clion_test.dir/main.c.o: CMakeFiles/Clion_test.dir/flags.make
CMakeFiles/Clion_test.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dvd/git/C_BitDefender/random_tests/Clion_test/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Clion_test.dir/main.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Clion_test.dir/main.c.o   -c /home/dvd/git/C_BitDefender/random_tests/Clion_test/main.c

CMakeFiles/Clion_test.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Clion_test.dir/main.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dvd/git/C_BitDefender/random_tests/Clion_test/main.c > CMakeFiles/Clion_test.dir/main.c.i

CMakeFiles/Clion_test.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Clion_test.dir/main.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dvd/git/C_BitDefender/random_tests/Clion_test/main.c -o CMakeFiles/Clion_test.dir/main.c.s

CMakeFiles/Clion_test.dir/main.c.o.requires:

.PHONY : CMakeFiles/Clion_test.dir/main.c.o.requires

CMakeFiles/Clion_test.dir/main.c.o.provides: CMakeFiles/Clion_test.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/Clion_test.dir/build.make CMakeFiles/Clion_test.dir/main.c.o.provides.build
.PHONY : CMakeFiles/Clion_test.dir/main.c.o.provides

CMakeFiles/Clion_test.dir/main.c.o.provides.build: CMakeFiles/Clion_test.dir/main.c.o


CMakeFiles/Clion_test.dir/function.c.o: CMakeFiles/Clion_test.dir/flags.make
CMakeFiles/Clion_test.dir/function.c.o: ../function.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dvd/git/C_BitDefender/random_tests/Clion_test/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Clion_test.dir/function.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Clion_test.dir/function.c.o   -c /home/dvd/git/C_BitDefender/random_tests/Clion_test/function.c

CMakeFiles/Clion_test.dir/function.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Clion_test.dir/function.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dvd/git/C_BitDefender/random_tests/Clion_test/function.c > CMakeFiles/Clion_test.dir/function.c.i

CMakeFiles/Clion_test.dir/function.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Clion_test.dir/function.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dvd/git/C_BitDefender/random_tests/Clion_test/function.c -o CMakeFiles/Clion_test.dir/function.c.s

CMakeFiles/Clion_test.dir/function.c.o.requires:

.PHONY : CMakeFiles/Clion_test.dir/function.c.o.requires

CMakeFiles/Clion_test.dir/function.c.o.provides: CMakeFiles/Clion_test.dir/function.c.o.requires
	$(MAKE) -f CMakeFiles/Clion_test.dir/build.make CMakeFiles/Clion_test.dir/function.c.o.provides.build
.PHONY : CMakeFiles/Clion_test.dir/function.c.o.provides

CMakeFiles/Clion_test.dir/function.c.o.provides.build: CMakeFiles/Clion_test.dir/function.c.o


# Object files for target Clion_test
Clion_test_OBJECTS = \
"CMakeFiles/Clion_test.dir/main.c.o" \
"CMakeFiles/Clion_test.dir/function.c.o"

# External object files for target Clion_test
Clion_test_EXTERNAL_OBJECTS =

Clion_test: CMakeFiles/Clion_test.dir/main.c.o
Clion_test: CMakeFiles/Clion_test.dir/function.c.o
Clion_test: CMakeFiles/Clion_test.dir/build.make
Clion_test: CMakeFiles/Clion_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dvd/git/C_BitDefender/random_tests/Clion_test/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable Clion_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Clion_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Clion_test.dir/build: Clion_test

.PHONY : CMakeFiles/Clion_test.dir/build

CMakeFiles/Clion_test.dir/requires: CMakeFiles/Clion_test.dir/main.c.o.requires
CMakeFiles/Clion_test.dir/requires: CMakeFiles/Clion_test.dir/function.c.o.requires

.PHONY : CMakeFiles/Clion_test.dir/requires

CMakeFiles/Clion_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Clion_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Clion_test.dir/clean

CMakeFiles/Clion_test.dir/depend:
	cd /home/dvd/git/C_BitDefender/random_tests/Clion_test/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dvd/git/C_BitDefender/random_tests/Clion_test /home/dvd/git/C_BitDefender/random_tests/Clion_test /home/dvd/git/C_BitDefender/random_tests/Clion_test/cmake-build-debug /home/dvd/git/C_BitDefender/random_tests/Clion_test/cmake-build-debug /home/dvd/git/C_BitDefender/random_tests/Clion_test/cmake-build-debug/CMakeFiles/Clion_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Clion_test.dir/depend

