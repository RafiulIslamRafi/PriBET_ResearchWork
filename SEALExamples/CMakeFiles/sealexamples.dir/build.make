# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kali/Downloads/SEAL_3.0/SEALExamples

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kali/Downloads/SEAL_3.0/SEALExamples

# Include any dependencies generated for this target.
include CMakeFiles/sealexamples.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/sealexamples.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/sealexamples.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sealexamples.dir/flags.make

CMakeFiles/sealexamples.dir/main.cpp.o: CMakeFiles/sealexamples.dir/flags.make
CMakeFiles/sealexamples.dir/main.cpp.o: main.cpp
CMakeFiles/sealexamples.dir/main.cpp.o: CMakeFiles/sealexamples.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kali/Downloads/SEAL_3.0/SEALExamples/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sealexamples.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/sealexamples.dir/main.cpp.o -MF CMakeFiles/sealexamples.dir/main.cpp.o.d -o CMakeFiles/sealexamples.dir/main.cpp.o -c /home/kali/Downloads/SEAL_3.0/SEALExamples/main.cpp

CMakeFiles/sealexamples.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sealexamples.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kali/Downloads/SEAL_3.0/SEALExamples/main.cpp > CMakeFiles/sealexamples.dir/main.cpp.i

CMakeFiles/sealexamples.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sealexamples.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kali/Downloads/SEAL_3.0/SEALExamples/main.cpp -o CMakeFiles/sealexamples.dir/main.cpp.s

# Object files for target sealexamples
sealexamples_OBJECTS = \
"CMakeFiles/sealexamples.dir/main.cpp.o"

# External object files for target sealexamples
sealexamples_EXTERNAL_OBJECTS =

/home/kali/Downloads/SEAL_3.0/bin/sealexamples: CMakeFiles/sealexamples.dir/main.cpp.o
/home/kali/Downloads/SEAL_3.0/bin/sealexamples: CMakeFiles/sealexamples.dir/build.make
/home/kali/Downloads/SEAL_3.0/bin/sealexamples: /usr/local/lib/libseal.a
/home/kali/Downloads/SEAL_3.0/bin/sealexamples: CMakeFiles/sealexamples.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kali/Downloads/SEAL_3.0/SEALExamples/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /home/kali/Downloads/SEAL_3.0/bin/sealexamples"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sealexamples.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sealexamples.dir/build: /home/kali/Downloads/SEAL_3.0/bin/sealexamples
.PHONY : CMakeFiles/sealexamples.dir/build

CMakeFiles/sealexamples.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sealexamples.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sealexamples.dir/clean

CMakeFiles/sealexamples.dir/depend:
	cd /home/kali/Downloads/SEAL_3.0/SEALExamples && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kali/Downloads/SEAL_3.0/SEALExamples /home/kali/Downloads/SEAL_3.0/SEALExamples /home/kali/Downloads/SEAL_3.0/SEALExamples /home/kali/Downloads/SEAL_3.0/SEALExamples /home/kali/Downloads/SEAL_3.0/SEALExamples/CMakeFiles/sealexamples.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sealexamples.dir/depend

