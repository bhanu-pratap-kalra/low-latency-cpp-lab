BUILD_TYPE ?= Release

BASELINE_DIR := new_delete_baseline
STAGE1_DIR := 01_big_object_pool
STAGE2_DIR := 02_templated_memory_pool
STAGE3_DIR := 03_raw_storage_alignment_and_object_lifetime

.PHONY: all configure build run run-example run-benchmark \
        clean rebuild help

all: build

configure:
	@echo "No parent-level CMake configuration required."

build:
	$(MAKE) -C $(STAGE1_DIR) build BUILD_TYPE=$(BUILD_TYPE)
	$(MAKE) -C $(STAGE2_DIR) build BUILD_TYPE=$(BUILD_TYPE)
	$(MAKE) -C $(STAGE3_DIR) build BUILD_TYPE=$(BUILD_TYPE)
	$(MAKE) -C $(BASELINE_DIR) build BUILD_TYPE=$(BUILD_TYPE)

#
# Parent-level run defaults to running the examples.
#
run: run-example

run-example:
	@echo ""
	@echo "========================================"
	@echo "Stage 1 - Simple Example"
	@echo "========================================"
	$(MAKE) -C $(STAGE1_DIR) run-example BUILD_TYPE=$(BUILD_TYPE)

	@echo ""
	@echo "========================================"
	@echo "Stage 2 - Simple Example"
	@echo "========================================"
	$(MAKE) -C $(STAGE2_DIR) run-example BUILD_TYPE=$(BUILD_TYPE)

	@echo ""
	@echo "========================================"
	@echo "Stage 3 - Simple Example"
	@echo "========================================"
	$(MAKE) -C $(STAGE3_DIR) run-example BUILD_TYPE=$(BUILD_TYPE)

#
# Run all benchmarks sequentially.
#
# They are intentionally not run in parallel because multiple benchmark
# processes competing for CPU/cache/memory resources could distort
# Average, P95 and P99 latency measurements.
#
run-benchmark:
	@echo ""
	@echo "========================================"
	@echo "Memory Pool Benchmark Suite"
	@echo "Build Type: $(BUILD_TYPE)"
	@echo "========================================"

	@echo ""
	@echo "========================================"
	@echo "Baseline - new/delete"
	@echo "========================================"
	$(MAKE) -C $(BASELINE_DIR) run-benchmark BUILD_TYPE=$(BUILD_TYPE)

	@echo ""
	@echo "========================================"
	@echo "Stage 1 - BigObjectMemoryPool"
	@echo "========================================"
	$(MAKE) -C $(STAGE1_DIR) run-benchmark BUILD_TYPE=$(BUILD_TYPE)

	@echo ""
	@echo "========================================"
	@echo "Stage 2 - MemoryPool<T>"
	@echo "========================================"
	$(MAKE) -C $(STAGE2_DIR) run-benchmark BUILD_TYPE=$(BUILD_TYPE)

	@echo ""
	@echo "========================================"
	@echo "Stage 3 - Raw Storage MemoryPool<T>"
	@echo "========================================"
	$(MAKE) -C $(STAGE3_DIR) run-benchmark BUILD_TYPE=$(BUILD_TYPE)

clean:
	$(MAKE) -C $(STAGE1_DIR) clean
	$(MAKE) -C $(STAGE2_DIR) clean
	$(MAKE) -C $(STAGE3_DIR) clean
	$(MAKE) -C $(BASELINE_DIR) clean

rebuild: clean build

help:
	@echo "Available commands:"
	@echo ""
	@echo "make"
	@echo "    Build all implementations in Release mode"
	@echo ""
	@echo "make run"
	@echo "    Run all simple examples"
	@echo ""
	@echo "make run-example"
	@echo "    Run all simple examples"
	@echo ""
	@echo "make run-benchmark"
	@echo "    Run all benchmarks sequentially"
	@echo ""
	@echo "make BUILD_TYPE=Debug"
	@echo "    Build all implementations in Debug mode"
	@echo ""
	@echo "make run-benchmark BUILD_TYPE=Debug"
	@echo "    Run all benchmarks in Debug mode"
	@echo ""
	@echo "make clean"
	@echo "    Remove all generated build directories"
	@echo ""
	@echo "make rebuild"
	@echo "    Clean and rebuild everything"