#include "BenchmarkUtils.h"
#include "BigObjectMemoryPool.h"

#include <cstddef>

int main()
{
    constexpr std::size_t Capacity = 1000;
    constexpr std::size_t WarmupCycles = 200;
    constexpr std::size_t MeasuredCycles = 5000;

    BigObjectMemoryPool big_object_memory_pool(Capacity);

    // Stage 1 benchmark:
    //
    // The pool preconstructs BigObject instances when the pool is created.
    //
    // acquire() returns an already existing BigObject from the pool.
    //
    // release() resets the object and returns its slot back to the free list.
    //
    // Pool construction is intentionally outside the timed region so this
    // benchmark measures steady-state acquire/release latency rather than
    // one-time pool setup cost.

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

    printStats("Stage 1 - BigObjectMemoryPool",result);

    return 0;
}