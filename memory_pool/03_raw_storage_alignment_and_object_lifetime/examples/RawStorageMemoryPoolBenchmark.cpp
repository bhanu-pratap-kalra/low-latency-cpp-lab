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

    // Stage 3 benchmark:
    //
    // The pool owns raw aligned storage.
    //
    // No BigObject exists in a free slot.
    //
    // acquire() starts the lifetime of a BigObject in an available slot
    // using placement new.
    //
    // release() explicitly destroys that BigObject and returns the slot
    // to the free list.
    //
    // Pool construction is intentionally outside the timed region so we
    // measure steady state acquire/release behaviour rather than one time
    // raw storage allocation cost.

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

    printStats("Stage 3 - Raw Storage MemoryPool<T>", result);

    return 0;
}