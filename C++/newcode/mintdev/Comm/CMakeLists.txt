cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories("/opt/newcode/mintdev/Comm/")

add_library("SharedLib"
   Functions/PkgBuff.cpp
   Functions/FixLenMem.cpp
   Functions/strfuncs.cpp
   Functions/Perf.cpp
   Functions/intmap.cpp
   Functions/synchronal.cpp
   Functions/encrypt.cpp
   Functions/Config.cpp
   MSocket/Buffer.cpp
   MSocket/MSocket.cpp
   Security/md5.cpp)

add_executable("binary" Main.cpp)

set_target_properties("binary" PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries("binary" "SharedLib")
