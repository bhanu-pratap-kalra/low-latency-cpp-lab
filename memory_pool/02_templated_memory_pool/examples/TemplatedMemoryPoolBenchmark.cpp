#include "BenchmarkUtils.h"
#include "BigObject.h"
#include "MemoryPool.h"

#include <cstddef>

int main()
{
    constexpr std::size_t Capacity = 1000;
    constexpr std::size_t WarmupCycles = 200;
    constexpr std::size_t MeasuredCycles = 5000;

    MemoryPool<BigObject> big_object_memory_pool(Capacity);

    // Stage 2 benchmark:
    //
    // The pool is generic, but all T objects are preconstructed when
    // the pool itself is created.
    //
    // acquire() returns an already existing T from the pool
    //
    // release() only marks the slot as free and returns its index to
    // the free list.
    //
    // IMPORTANT:
    //
    // Stage 2 does NOT reset or destroy the object when it is released
    // The T object remains alive and may still contain its previous state
    //
    // This benchmark therefore measures the Stage 2 implementation
    // exactly as designed: mainly free list and in use bookkeeping
    //
    // Its results are not semantically identical to Stage 1, Stage 3,
    // or new/delete because those implementations perform additional
    // object reset or lifetime management work.
    //
    // Pool construction is outside the timed region.

    const auto result = benchmarkPool(
        Capacity,
        WarmupCycles,
        MeasuredCycles,

        [&big_object_memory_pool]()
        {
            return big_object_memory_pool.acquire();
        },

        [&big_object_memory_pool](BigObject* object)
        {
            big_object_memory_pool.release(object);
        }
    );

    printStats(
        "Stage 2 - Preconstructed MemoryPool<T>",
        result
    );

    return 0;
}