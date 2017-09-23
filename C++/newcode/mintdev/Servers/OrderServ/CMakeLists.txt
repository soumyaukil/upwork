cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories("/opt/newcode/mintdev/Servers/OrderServ/include")
include_directories("/opt/newcode/mintdev/Comm")
add_library("OrderServer" 
  src/OrderServ.cpp
  src/OrderConfig.cpp
  src/ClientPortDaemon.cpp)
