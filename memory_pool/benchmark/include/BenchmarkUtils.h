#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

struct LatencyStats
{
    double average_ns{};
    double p95_ns{};
    double p99_ns{};
};

struct PoolBenchmarkResult
{
    LatencyStats acquire;
    LatencyStats release;
    LatencyStats acquire_release;
};

inline LatencyStats calculateStats(std::vector<double> samples)
{
    if (samples.empty())
    {
        return {};
    }

    const double total = std::accumulate(samples.begin(), samples.end(), 0.0);

    const double average = total / static_cast<double>(samples.size());

    std::sort(samples.begin(), samples.end());

    const auto percentile = [&samples](double p)
    {
        const std::size_t index = static_cast<std::size_t>(
                            std::ceil( p * static_cast<double>(samples.size()))
                            ) 
                            - 1;

        return samples[std::min(index, samples.size() - 1)];
    };

    return { average, percentile(0.95), percentile(0.99) };
}

template <typename AcquireFunction,
          typename ReleaseFunction>
PoolBenchmarkResult benchmarkPool(
    std::size_t capacity,
    std::size_t warmup_cycles,
    std::size_t measured_cycles,
    AcquireFunction acquire,
    ReleaseFunction release)
{
    using Clock = std::chrono::steady_clock;

    using Pointer = decltype(acquire());

    std::vector<Pointer> objects(capacity);

    // Why Warm up phase?
    //
    // Run a number of acquire/release cycles before recording measurements.
    //
    // The first few iterations can be noisier because the CPU caches, branch
    // predictors, memory pages, allocator state, and code paths may not yet be
    // in a steady execution state.
    //
    // Warming up helps reduce one time startup effects so the measured
    // Average, P95, and P99 better represent steady state pool behaviour
    for (std::size_t cycle = 0; cycle < warmup_cycles; ++cycle)
    {
        for (std::size_t i = 0; i < capacity; ++i)
        {
            objects[i] = acquire();
        }

        for (std::size_t i = 0; i < capacity; ++i)
        {
            release(objects[i]);
        }
    }

    std::vector<double> acquire_samples;
    std::vector<double> release_samples;
    std::vector<double> cycle_samples;

    acquire_samples.reserve(measured_cycles);
    release_samples.reserve(measured_cycles);
    cycle_samples.reserve(measured_cycles);

    // Measure operations in batches rather than timing every acquire/release individually.
    //
    // A single pool operation may take only a few nanoseconds, so calling the
    // clock around every operation could add significant measurement overhead.
    //
    // We therefore time capacity operations together and divide the elapsed
    // time by capacity to estimate the average cost per operation for this
    // benchmark cycle.
    for (std::size_t cycle = 0; cycle < measured_cycles; ++cycle)
    {
        const auto acquire_start = Clock::now();

        for (std::size_t i = 0; i < capacity; ++i)
        {
            objects[i] = acquire();
        }

        const auto acquire_end = Clock::now();

        for (std::size_t i = 0; i < capacity; ++i)
        {
            release(objects[i]);
        }

        const auto release_end = Clock::now();

        const double acquire_ns = std::chrono::duration<double, std::nano>(acquire_end - acquire_start).count();

        const double release_ns = std::chrono::duration<double, std::nano>(release_end - acquire_end).count();

        const double total_ns = std::chrono::duration<double, std::nano>(release_end - acquire_start).count();

        //
        // Store latency PER OBJECT.
        //
        acquire_samples.push_back(acquire_ns / capacity);

        release_samples.push_back(release_ns / capacity);

        //
        // One acquire + one release pair per object.
        //
        cycle_samples.push_back(total_ns / capacity);
    }

    return {calculateStats(std::move(acquire_samples)), calculateStats(std::move(release_samples)), calculateStats(std::move(cycle_samples))};
}

inline void printStats(
    const std::string& name,
    const PoolBenchmarkResult& result)
{
    std::cout << std::endl << name << std::endl;

    std::cout
        << std::left
        << std::setw(22)
        << "Operation"
        << std::right
        << std::setw(14)
        << "Average(ns)"
        << std::setw(14)
        << "P95(ns)"
        << std::setw(14)
        << "P99(ns)"
        << std::endl;

    std::cout << std::string(64, '-') << std::endl;

    const auto print = [](const std::string& operation, const LatencyStats& stats)
    {
        std::cout
            << std::left
            << std::setw(22)
            << operation
            << std::right
            << std::fixed
            << std::setprecision(2)
            << std::setw(14)
            << stats.average_ns
            << std::setw(14)
            << stats.p95_ns
            << std::setw(14)
            << stats.p99_ns
            << '\n';
    };

    print("Acquire", result.acquire);
    print("Release", result.release);
    print("Acquire + Release", result.acquire_release);
}