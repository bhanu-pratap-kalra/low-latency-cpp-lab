cmake_minimum_required(VERSION 3.20)

project(RawStorageMemoryPool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(RawStorageMemoryPoolSimpleExample
    examples/RawStorageMemoryPoolSimpleExample.cpp
)

target_include_directories(RawStorageMemoryPoolSimpleExample
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(RawStorageMemoryPoolSimpleExample
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)
