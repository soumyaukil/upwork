# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /opt/newcode/mintdev/Servers/OrderServ

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /opt/newcode/mintdev/Servers/OrderServ/build

# Include any dependencies generated for this target.
include CMakeFiles/OrderServer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/OrderServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/OrderServer.dir/flags.make

CMakeFiles/OrderServer.dir/src/OrderServ.cpp.o: CMakeFiles/OrderServer.dir/flags.make
CMakeFiles/OrderServer.dir/src/OrderServ.cpp.o: ../src/OrderServ.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /opt/newcode/mintdev/Servers/OrderServ/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/OrderServer.dir/src/OrderServ.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/OrderServer.dir/src/OrderServ.cpp.o -c /opt/newcode/mintdev/Servers/OrderServ/src/OrderServ.cpp

CMakeFiles/OrderServer.dir/src/OrderServ.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/OrderServer.dir/src/OrderServ.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /opt/newcode/mintdev/Servers/OrderServ/src/OrderServ.cpp > CMakeFiles/OrderServer.dir/src/OrderServ.cpp.i

CMakeFiles/OrderServer.dir/src/OrderServ.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/OrderServer.dir/src/OrderServ.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /opt/newcode/mintdev/Servers/OrderServ/src/OrderServ.cpp -o CMakeFiles/OrderServer.dir/src/OrderServ.cpp.s

CMakeFiles/OrderServer.dir/src/OrderServ.cpp.o.requires:
.PHONY : CMakeFiles/OrderServer.dir/src/OrderServ.cpp.o.requires

CMakeFiles/OrderServer.dir/src/OrderServ.cpp.o.provides: CMakeFiles/OrderServer.dir/src/OrderServ.cpp.o.requires
	$(MAKE) -f CMakeFiles/OrderServer.dir/build.make CMakeFiles/OrderServer.dir/src/OrderServ.cpp.o.provides.build
.PHONY : CMakeFiles/OrderServer.dir/src/OrderServ.cpp.o.provides

CMakeFiles/OrderServer.dir/src/OrderServ.cpp.o.provides.build: CMakeFiles/OrderServer.dir/src/OrderServ.cpp.o

CMakeFiles/OrderServer.dir/src/OrderConfig.cpp.o: CMakeFiles/OrderServer.dir/flags.make
CMakeFiles/OrderServer.dir/src/OrderConfig.cpp.o: ../src/OrderConfig.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /opt/newcode/mintdev/Servers/OrderServ/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/OrderServer.dir/src/OrderConfig.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/OrderServer.dir/src/OrderConfig.cpp.o -c /opt/newcode/mintdev/Servers/OrderServ/src/OrderConfig.cpp

CMakeFiles/OrderServer.dir/src/OrderConfig.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/OrderServer.dir/src/OrderConfig.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /opt/newcode/mintdev/Servers/OrderServ/src/OrderConfig.cpp > CMakeFiles/OrderServer.dir/src/OrderConfig.cpp.i

CMakeFiles/OrderServer.dir/src/OrderConfig.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/OrderServer.dir/src/OrderConfig.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /opt/newcode/mintdev/Servers/OrderServ/src/OrderConfig.cpp -o CMakeFiles/OrderServer.dir/src/OrderConfig.cpp.s

CMakeFiles/OrderServer.dir/src/OrderConfig.cpp.o.requires:
.PHONY : CMakeFiles/OrderServer.dir/src/OrderConfig.cpp.o.requires

CMakeFiles/OrderServer.dir/src/OrderConfig.cpp.o.provides: CMakeFiles/OrderServer.dir/src/OrderConfig.cpp.o.requires
	$(MAKE) -f CMakeFiles/OrderServer.dir/build.make CMakeFiles/OrderServer.dir/src/OrderConfig.cpp.o.provides.build
.PHONY : CMakeFiles/OrderServer.dir/src/OrderConfig.cpp.o.provides

CMakeFiles/OrderServer.dir/src/OrderConfig.cpp.o.provides.build: CMakeFiles/OrderServer.dir/src/OrderConfig.cpp.o

CMakeFiles/OrderServer.dir/src/ClientPortDaemon.cpp.o: CMakeFiles/OrderServer.dir/flags.make
CMakeFiles/OrderServer.dir/src/ClientPortDaemon.cpp.o: ../src/ClientPortDaemon.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /opt/newcode/mintdev/Servers/OrderServ/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/OrderServer.dir/src/ClientPortDaemon.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/OrderServer.dir/src/ClientPortDaemon.cpp.o -c /opt/newcode/mintdev/Servers/OrderServ/src/ClientPortDaemon.cpp

CMakeFiles/OrderServer.dir/src/ClientPortDaemon.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/OrderServer.dir/src/ClientPortDaemon.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /opt/newcode/mintdev/Servers/OrderServ/src/ClientPortDaemon.cpp > CMakeFiles/OrderServer.dir/src/ClientPortDaemon.cpp.i

CMakeFiles/OrderServer.dir/src/ClientPortDaemon.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/OrderServer.dir/src/ClientPortDaemon.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /opt/newcode/mintdev/Servers/OrderServ/src/ClientPortDaemon.cpp -o CMakeFiles/OrderServer.dir/src/ClientPortDaemon.cpp.s

CMakeFiles/OrderServer.dir/src/ClientPortDaemon.cpp.o.requires:
.PHONY : CMakeFiles/OrderServer.dir/src/ClientPortDaemon.cpp.o.requires

CMakeFiles/OrderServer.dir/src/ClientPortDaemon.cpp.o.provides: CMakeFiles/OrderServer.dir/src/ClientPortDaemon.cpp.o.requires
	$(MAKE) -f CMakeFiles/OrderServer.dir/build.make CMakeFiles/OrderServer.dir/src/ClientPortDaemon.cpp.o.provides.build
.PHONY : CMakeFiles/OrderServer.dir/src/ClientPortDaemon.cpp.o.provides

CMakeFiles/OrderServer.dir/src/ClientPortDaemon.cpp.o.provides.build: CMakeFiles/OrderServer.dir/src/ClientPortDaemon.cpp.o

# Object files for target OrderServer
OrderServer_OBJECTS = \
"CMakeFiles/OrderServer.dir/src/OrderServ.cpp.o" \
"CMakeFiles/OrderServer.dir/src/OrderConfig.cpp.o" \
"CMakeFiles/OrderServer.dir/src/ClientPortDaemon.cpp.o"

# External object files for target OrderServer
OrderServer_EXTERNAL_OBJECTS =

libOrderServer.a: CMakeFiles/OrderServer.dir/src/OrderServ.cpp.o
libOrderServer.a: CMakeFiles/OrderServer.dir/src/OrderConfig.cpp.o
libOrderServer.a: CMakeFiles/OrderServer.dir/src/ClientPortDaemon.cpp.o
libOrderServer.a: CMakeFiles/OrderServer.dir/build.make
libOrderServer.a: CMakeFiles/OrderServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libOrderServer.a"
	$(CMAKE_COMMAND) -P CMakeFiles/OrderServer.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/OrderServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/OrderServer.dir/build: libOrderServer.a
.PHONY : CMakeFiles/OrderServer.dir/build

CMakeFiles/OrderServer.dir/requires: CMakeFiles/OrderServer.dir/src/OrderServ.cpp.o.requires
CMakeFiles/OrderServer.dir/requires: CMakeFiles/OrderServer.dir/src/OrderConfig.cpp.o.requires
CMakeFiles/OrderServer.dir/requires: CMakeFiles/OrderServer.dir/src/ClientPortDaemon.cpp.o.requires
.PHONY : CMakeFiles/OrderServer.dir/requires

CMakeFiles/OrderServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/OrderServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/OrderServer.dir/clean

CMakeFiles/OrderServer.dir/depend:
	cd /opt/newcode/mintdev/Servers/OrderServ/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /opt/newcode/mintdev/Servers/OrderServ /opt/newcode/mintdev/Servers/OrderServ /opt/newcode/mintdev/Servers/OrderServ/build /opt/newcode/mintdev/Servers/OrderServ/build /opt/newcode/mintdev/Servers/OrderServ/build/CMakeFiles/OrderServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/OrderServer.dir/depend

