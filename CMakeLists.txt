cmake_minimum_required(VERSION 2.8)

project(ping-cpp)
add_library(ping-cpp INTERFACE)

set(PING_CPP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/message/)

include_directories(${CMAKE_SOURCE_DIR} ${PING_CPP_INCLUDE_DIR})
target_include_directories(ping-cpp INTERFACE ${PING_CPP_INCLUDE_DIR})
target_link_libraries(ping-cpp)
