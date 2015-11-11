cmake_minimum_required(VERSION 2.6)
project(pak)

add_executable(pak exceptionhandler.cpp 
func.cpp main.cpp pak.cpp directoryentry.cpp
treeitem.cpp pakexception.cpp)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCLI")

install(TARGETS pak RUNTIME DESTINATION bin)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()