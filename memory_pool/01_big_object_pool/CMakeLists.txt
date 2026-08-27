cmake_minimum_required(VERSION 3.20)

project(BigObjectMemoryPool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#
# BigObjectMemoryPool library
#
add_library(
    BigObjectMemoryPool
    src/BigObjectMemoryPool.cpp
)

target_include_directories(
    BigObjectMemoryPool
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../common/include
)

target_compile_options(
    BigObjectMemoryPool
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

#
# Simple example
#
add_executable(
    BigObjectMemoryPoolSimpleExample
    examples/BigObjectMemoryPoolSimpleExample.cpp
)

target_link_libraries(
    BigObjectMemoryPoolSimpleExample
    PRIVATE
        BigObjectMemoryPool
)

#
# Benchmark
#
add_executable(
    BigObjectMemoryPoolBenchmark
    examples/BigObjectMemoryPoolBenchmark.cpp
)

target_include_directories(
    BigObjectMemoryPoolBenchmark
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../benchmark/include
)

target_link_libraries(
    BigObjectMemoryPoolBenchmark
    PRIVATE
        BigObjectMemoryPool
)