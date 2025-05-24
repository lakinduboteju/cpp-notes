cmake_minimum_required(VERSION 3.10)
project(producer-consumer)

set(CMAKE_CXX_STANDARD 17)

# Single producer-consumer demo
add_executable(single-producer-consumer single_producer_consumer.cpp)

# Multi producer-consumer demo
add_executable(multi-producer-consumer multi_producer_consumer.cpp)