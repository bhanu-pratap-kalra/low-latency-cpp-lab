BUILD_DIR := build
BUILD_TYPE ?= Debug

.PHONY: all build run clean rebuild help

all: build

configure:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

build: configure
	cmake --build $(BUILD_DIR)

run: build
	./$(BUILD_DIR)/TemplatedMemoryPoolSimpleExample

clean:
	cmake -E remove_directory $(BUILD_DIR)

rebuild: clean build

help:
	@echo " Available commands:"
	@echo " make                    Configure and build"
	@echo " make run                Build and run the example"
	@echo " make clean              Remove generated build files"
	@echo " make rebuild            Clean and rebuild"
	@echo " make BUILD_TYPE=Release Release build"
