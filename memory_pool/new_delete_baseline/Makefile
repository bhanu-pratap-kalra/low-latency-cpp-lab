BUILD_DIR := build

BUILD_TYPE ?= Release

.PHONY: all configure build run run-benchmark clean rebuild help

all: build

configure:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

build: configure
	cmake --build $(BUILD_DIR)

run: run-benchmark

run-benchmark: build
	./$(BUILD_DIR)/NewDeleteSimpleExample

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean build

help:
	@echo "Available commands:"
	@echo ""
	@echo "make"
	@echo "    Configure and build in Release mode"
	@echo ""
	@echo "make run"
	@echo "    Build and run the benchmark"
	@echo ""
	@echo "make run-benchmark"
	@echo "    Build and run the benchmark"
	@echo ""
	@echo "make BUILD_TYPE=Debug"
	@echo "    Build in Debug mode"
	@echo ""
	@echo "make run-benchmark BUILD_TYPE=Debug"
	@echo "    Run benchmark in Debug mode"
	@echo ""
	@echo "make clean"
	@echo "    Remove build directory"
	@echo ""
	@echo "make rebuild"
	@echo "    Clean and rebuild"