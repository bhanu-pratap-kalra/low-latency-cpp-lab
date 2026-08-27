BUILD_DIR := build

BUILD_TYPE ?= Release

.PHONY: all configure build run run-example run-benchmark clean rebuild help

all: build

configure:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

build: configure
	cmake --build $(BUILD_DIR)

run: run-example

run-example: build
	./$(BUILD_DIR)/RawStorageMemoryPoolSimpleExample

run-benchmark: build
	./$(BUILD_DIR)/RawStorageMemoryPoolBenchmark

clean:
	cmake -E remove_directory $(BUILD_DIR)

rebuild: clean build

help:
	@echo "Available commands:"
	@echo ""
	@echo "make                         Configure and build in Release mode"
	@echo "make run                     Build and run the simple example"
	@echo "make run-example             Build and run the simple example"
	@echo "make run-benchmark           Build and run the benchmark"
	@echo "make BUILD_TYPE=Debug        Build in Debug mode"
	@echo "make run BUILD_TYPE=Debug    Run the simple example in Debug mode"
	@echo "make run-benchmark BUILD_TYPE=Debug"
	@echo "                             Run the benchmark in Debug mode"
	@echo "make clean                   Remove generated build files"
	@echo "make rebuild                 Clean and rebuild"