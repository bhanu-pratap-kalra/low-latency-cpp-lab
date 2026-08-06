cmake_minimum_required(VERSION 3.20)

project(BigObjectMemoryPool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(BigObjectMemoryPool
	src/BigObjectMemoryPool.cpp
)

target_include_directories(BigObjectMemoryPool
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(BigObjectMemoryPool
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

add_executable(BigObjectMemoryPoolSimpleExample
	examples/BigObjectMemoryPoolSimpleExample.cpp
)

target_link_libraries(BigObjectMemoryPoolSimpleExample
    PRIVATE
    BigObjectMemoryPool
)
