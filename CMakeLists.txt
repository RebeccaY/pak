cmake_minimum_required(VERSION 2.6)
project(pak)

add_executable(pak main.cpp)

install(TARGETS pak RUNTIME DESTINATION bin)
