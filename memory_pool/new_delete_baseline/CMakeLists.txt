cmake_minimum_required(VERSION 3.20)

project(NewDeleteBaseline LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(
    NewDeleteSimpleExample
    examples/NewDeleteSimpleExample.cpp
)

target_include_directories(
    NewDeleteSimpleExample
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../common/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../benchmark/include
)

target_compile_options(
    NewDeleteSimpleExample
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)