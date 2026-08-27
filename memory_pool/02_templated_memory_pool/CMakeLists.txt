cmake_minimum_required(VERSION 3.20)

project(TemplatedMemoryPool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#
# Simple example
#
add_executable(
    TemplatedMemoryPoolSimpleExample
    examples/TemplatedMemoryPoolSimpleExample.cpp
)

target_include_directories(
    TemplatedMemoryPoolSimpleExample
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../common/include
)

target_compile_options(
    TemplatedMemoryPoolSimpleExample
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

#
# Benchmark
#
add_executable(
    TemplatedMemoryPoolBenchmark
    examples/TemplatedMemoryPoolBenchmark.cpp
)

target_include_directories(
    TemplatedMemoryPoolBenchmark
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../common/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../benchmark/include
)

target_compile_options(
    TemplatedMemoryPoolBenchmark
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)