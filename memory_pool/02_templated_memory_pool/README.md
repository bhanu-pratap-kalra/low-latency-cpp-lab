# Templated Memory Pool

Stage 2 evolves the original `BigObjectMemoryPool` into a reusable memory pool capable of managing different object types.

The key change is the introduction of a class template:

```cpp
template <typename T>
class MemoryPool;
