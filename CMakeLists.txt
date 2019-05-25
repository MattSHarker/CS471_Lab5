
cmake_minimum_required(VERSION 3.1.0)
set(CMAKE_CXX_STANDARD 11)

project(cs471_proj_5)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

find_package(Threads REQUIRED)
include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(cs471_proj_5.out ${SOURCES})
target_link_libraries (cs471_proj_5.out ${CMAKE_THREAD_LIBS_INIT})
