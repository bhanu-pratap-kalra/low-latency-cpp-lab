cmake_minimum_required(VERSION 3.20)

project(RawStorageMemoryPool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#
# Simple example
#
add_executable(
    RawStorageMemoryPoolSimpleExample
    examples/RawStorageMemoryPoolSimpleExample.cpp
)

target_include_directories(
    RawStorageMemoryPoolSimpleExample
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../common/include
)

target_compile_options(
    RawStorageMemoryPoolSimpleExample
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

#
# Benchmark
#
add_executable(
    RawStorageMemoryPoolBenchmark
    examples/RawStorageMemoryPoolBenchmark.cpp
)

target_include_directories(
    RawStorageMemoryPoolBenchmark
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../common/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../benchmark/include
)

target_compile_options(
    RawStorageMemoryPoolBenchmark
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)