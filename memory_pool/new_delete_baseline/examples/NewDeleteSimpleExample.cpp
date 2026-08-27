#include "BenchmarkUtils.h"
#include "BigObject.h"

#include <cstddef>

int main()
{
    constexpr std::size_t Capacity = 1000;
    constexpr std::size_t WarmupCycles = 200;
    constexpr std::size_t MeasuredCycles = 5000;

    // Baseline benchmark:
    //
    // No memory pool is used here.
    //
    // acquire() performs normal dynamic allocation + construction
    // using `new BigObject`.
    //
    // release() performs normal destruction + deallocation
    // using `delete`.
    //
    // This gives us a baseline against which the memory pool
    // implementations can be compared.

    const auto result = benchmarkPool(
        Capacity, 
        WarmupCycles, 
        MeasuredCycles,
        []()
        {
            return new BigObject{};
        },

        [](BigObject* big_object)
        {
            delete big_object;
        }
    );

    printStats("Baseline - new/delete", result);

    return 0;
}