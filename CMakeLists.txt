cmake_minimum_required(VERSION 2.6)
project(boosttest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Boost 1.40 COMPONENTS program_options system REQUIRED)
find_package(Threads)
include_directories(${Boost_INCLUDE_DIR})

add_executable(server server.cpp)
target_link_libraries(server LINK_PUBLIC ${Boost_LIBRARIES})

add_executable(client client.cpp)
target_link_libraries(client LINK_PUBLIC ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
