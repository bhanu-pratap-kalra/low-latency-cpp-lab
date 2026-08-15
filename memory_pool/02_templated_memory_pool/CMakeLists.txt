cmake_minimum_required(VERSION 3.20)

project(TemplatedMemoryPool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(TemplatedMemoryPoolSimpleExample
    examples/TemplatedMemoryPoolSimpleExample.cpp
)

target_include_directories(TemplatedMemoryPoolSimpleExample
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(TemplatedMemoryPoolSimpleExample
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)
